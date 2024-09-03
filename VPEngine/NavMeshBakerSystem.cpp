#include "pch.h"
#include "NavMeshBakerSystem.h"

NavMeshBakerSystem::NavMeshBakerSystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{
	if (m_NavMeshData != nullptr)
	{
		delete m_NavMeshData;
	}
}

void NavMeshBakerSystem::MakeNavigationMesh(BuildSettings buildSettrings)
{

	if (m_NavMeshData != nullptr)
		delete m_NavMeshData;
	if (!buildSettrings.UseNavMesh)
		return;
	m_NavMeshData = new NavMeshData(this);

	std::vector<VPMath::Vector3> worldVertices;
	std::vector<int> worldFaces;

	m_PhysicsEngine->ExtractVerticesAndFaces(static_cast<int>(VPPhysics::EPhysicsLayer::GROUND), worldVertices, worldFaces);
	m_PhysicsEngine->ExtractVerticesAndFaces(static_cast<int>(VPPhysics::EPhysicsLayer::WALL), worldVertices, worldFaces);
}

void NavMeshBakerSystem::makeNavMesh(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const BuildSettings& buildSettings)
{
	float bmin[3]{ FLT_MAX, FLT_MAX, FLT_MAX };
	float bmax[3]{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

	// 바운더리 정보부터 설정
	for (auto i = 0; i < verticesNum; i++)
	{
		if (bmin[0] > worldVertices[i * 3])
			bmin[0] = worldVertices[i * 3];
		if (bmin[1] > worldVertices[i * 3 + 1])
			bmin[1] = worldVertices[i * 3 + 1];
		if (bmin[2] > worldVertices[i * 3 + 2])
			bmin[2] = worldVertices[i * 3 + 2];

		if (bmax[0] < worldVertices[i * 3])
			bmax[0] = worldVertices[i * 3];
		if (bmax[1] < worldVertices[i * 3 + 1])
			bmax[1] = worldVertices[i * 3 + 1];
		if (bmax[2] < worldVertices[i * 3 + 2])
			bmax[2] = worldVertices[i * 3 + 2];
	}
	auto& config{ m_NavMeshData->config };
	memset(&config, 0, sizeof(rcConfig));

	config.cs = buildSettings.divisionSizeXZ;
	config.ch = buildSettings.divisionSizeY;
	config.walkableSlopeAngle = buildSettings.walkableSlopeAngle;
	config.walkableHeight = (int)ceilf(buildSettings.walkableHeight / config.ch);
	config.walkableClimb = (int)floorf(buildSettings.walkableClimb / config.ch);
	config.walkableRadius = (int)ceilf(config.cs * 2 / config.cs);
	config.maxEdgeLen = (int)(config.cs * 40 / config.cs);
	config.maxSimplificationError = 1.3f;
	config.minRegionArea = (int)rcSqr(config.cs * 27);		// Note: area = size*size
	config.mergeRegionArea = (int)rcSqr(config.cs * 67);	// Note: area = size*size
	config.maxVertsPerPoly = (int)6;
	config.detailSampleDist = 6.0f < 0.9f ? 0 : config.cs * 6.0f;
	config.detailSampleMaxError = config.ch * 1;

	rcVcopy(config.bmin, bmin);
	rcVcopy(config.bmax, bmax);
	rcCalcGridSize(config.bmin, config.bmax, config.cs, &config.width, &config.height);

	// 작업 맥락을 저장할 context 객체 생성, 작업의 성패여부를 저장할 processResult 선언
	auto* context = m_NavMeshData->context.get();
	bool processResult{ false };
	// 복셀 높이필드 공간 할당
	rcHeightfield* heightField{ rcAllocHeightfield() };
	assert(heightField != nullptr);

	processResult = rcCreateHeightfield(context, *heightField, config.width, config.height, config.bmin, config.bmax, config.cs, config.ch);
	assert(processResult == true);

	std::vector<unsigned char> triareas;
	triareas.resize(facesNum);
	//unsigned char * triareas = new unsigned char[facesNum];
	//memset(triareas, 0, facesNum*sizeof(unsigned char));

	rcMarkWalkableTriangles(context, config.walkableSlopeAngle, worldVertices, verticesNum, faces, facesNum, triareas.data());
	processResult = rcRasterizeTriangles(context, worldVertices, verticesNum, faces, triareas.data(), facesNum, *heightField, config.walkableClimb);
	assert(processResult == true);

	// 필요없는 부분 필터링
	rcFilterLowHangingWalkableObstacles(context, config.walkableClimb, *heightField);
	rcFilterLedgeSpans(context, config.walkableHeight, config.walkableClimb, *heightField);
	rcFilterWalkableLowHeightSpans(context, config.walkableHeight, *heightField);

	// 밀집 높이 필드 만들기
	rcCompactHeightfield* compactHeightField{ rcAllocCompactHeightfield() };
	assert(compactHeightField != nullptr);

	processResult = rcBuildCompactHeightfield(context, config.walkableHeight, config.walkableClimb, *heightField, *compactHeightField);
	//rcFreeHeightField(heightField);
	assert(processResult == true);

	//processResult = rcErodeWalkableArea(context, config.walkableRadius, *compactHeightField);
	//assert(processResult == true);

	processResult = rcBuildDistanceField(context, *compactHeightField);
	assert(processResult == true);

	rcBuildRegions(context, *compactHeightField, 0, config.minRegionArea, config.mergeRegionArea);
	assert(processResult == true);

	// 윤곽선 만들기
	rcContourSet* contourSet{ rcAllocContourSet() };
	assert(contourSet != nullptr);

	processResult = rcBuildContours(context, *compactHeightField, config.maxSimplificationError, config.maxEdgeLen, *contourSet);
	assert(processResult == true);

	// 윤곽선으로부터 폴리곤 생성
	rcPolyMesh*& polyMesh{ m_NavMeshData->polyMesh = rcAllocPolyMesh() };
	assert(polyMesh != nullptr);

	processResult = rcBuildPolyMesh(context, *contourSet, config.maxVertsPerPoly, *polyMesh);
	assert(processResult == true);

	// 디테일 메시 생성
	auto& detailMesh{ m_NavMeshData->polyMeshDetail = rcAllocPolyMeshDetail() };
	assert(detailMesh != nullptr);

	processResult = rcBuildPolyMeshDetail(context, *polyMesh, *compactHeightField, config.detailSampleDist, config.detailSampleMaxError, *detailMesh);
	assert(processResult == true);

	//rcFreeCompactHeightfield(compactHeightField);
	//rcFreeContourSet(contourSet);

	// detour 데이터 생성
	unsigned char* navData{ nullptr };
	int navDataSize{ 0 };

	assert(config.maxVertsPerPoly <= DT_VERTS_PER_POLYGON);

	// Update poly flags from areas.
	for (int i = 0; i < polyMesh->npolys; ++i)
	{
		if (polyMesh->areas[i] == RC_WALKABLE_AREA)
		{
			polyMesh->areas[i] = 0;
			polyMesh->flags[i] = 1;
		}
	}
	dtNavMeshCreateParams params;
	memset(&params, 0, sizeof(params));
	params.verts = polyMesh->verts;
	params.vertCount = polyMesh->nverts;
	params.polys = polyMesh->polys;
	params.polyAreas = polyMesh->areas;
	params.polyFlags = polyMesh->flags;
	params.polyCount = polyMesh->npolys;
	params.nvp = polyMesh->nvp;
	params.detailMeshes = detailMesh->meshes;
	params.detailVerts = detailMesh->verts;
	params.detailVertsCount = detailMesh->nverts;
	params.detailTris = detailMesh->tris;
	params.detailTriCount = detailMesh->ntris;
	params.offMeshConVerts = 0;
	params.offMeshConRad = 0;
	params.offMeshConDir = 0;
	params.offMeshConAreas = 0;
	params.offMeshConFlags = 0;
	params.offMeshConUserID = 0;
	params.offMeshConCount = 0;
	params.walkableHeight = config.walkableHeight;
	params.walkableRadius = config.walkableRadius;
	params.walkableClimb = config.walkableClimb;
	rcVcopy(params.bmin, polyMesh->bmin);
	rcVcopy(params.bmax, polyMesh->bmax);
	params.cs = config.cs;
	params.ch = config.ch;
	params.buildBvTree = true;

	processResult = dtCreateNavMeshData(&params, &navData, &navDataSize);
	assert(processResult == true);

	dtNavMesh* navMesh{ m_NavMeshData->navMesh = dtAllocNavMesh() };
	assert(navMesh != nullptr);

	dtStatus status;
	status = navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
	//dtFree(navData);
	assert(dtStatusFailed(status) == false);

	dtNavMeshQuery* navQuery{ m_NavMeshData->navQuery };
	status = navQuery->init(navMesh, 2048);
	assert(dtStatusFailed(status) == false);

	m_NavMeshData->crowd->init(1024, buildSettings.maxAgentRadius, navMesh);
}

void NavMeshBakerSystem::Update(float deltaTime)
{
	if (m_NavMeshData->crowd == nullptr)
		return;
	if (m_NavMeshData->crowd->getAgentCount() != 0)
	{
		m_NavMeshData->crowd->update(deltaTime, nullptr);
	}
}

void NavMeshBakerSystem::PhysicsUpdate(float deltaTime)
{
}

void NavMeshBakerSystem::Initialize()
{
	MakeNavigationMesh(GetSceneManager()->GetSceneBuildSettrings());
}

void NavMeshBakerSystem::Start(uint32_t gameObjectId)
{
}

void NavMeshBakerSystem::Finish(uint32_t gameObjectId)
{
}

void NavMeshBakerSystem::Finalize()
{
}


