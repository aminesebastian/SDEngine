#pragma once
#include "Transform.h"
#include "World.h"

#define	MAX_QPATH			64	

typedef struct model_s {
	char		name[MAX_QPATH];

	int			registration_sequence;

	//modtype_t	type; //Need to touch on later
	int			numframes;

	int			flags;

	//
	// volume occupied by the model graphics
	//		
	glm::vec3		mins, maxs;
	float		radius;

	//
	// solid volume for clipping 
	//
	bool	clipbox;
	glm::vec3		clipmins, clipmaxs;

	// BSP NEEDS TO BE WORKED ON.
	// brush model
	//
	//int			firstmodelsurface, nummodelsurfaces;
	//int			lightmap;		// only for submodels

	//int			numsubmodels;
	//mmodel_t	*submodels; // 

	//int			numplanes;
	//cplane_t	*planes;

	//int			numleafs;		// number of visible leafs, not counting 0
	//mleaf_t		*leafs;

	//int			numvertexes;
	//mvertex_t	*vertexes;

	//int			numedges;
	//medge_t		*edges;

	//int			numnodes;
	//int			firstnode;
	//mnode_t		*nodes;

	//int			numtexinfo;
	//mtexinfo_t	*texinfo;

	//int			numsurfaces;
	//msurface_t	*surfaces;

	//int			numsurfedges;
	//int			*surfedges;

	//int			nummarksurfaces;
	//msurface_t	**marksurfaces;

	//dvis_t		*vis;

	//byte		*lightdata;

	//// for alias models and skins
	//image_t		*skins[MAX_MD2SKINS];

	int			extradatasize;
	void		*extradata;
} model_t;

typedef struct {
	glm::vec3 origin;
	int		color;
	float	alpha;
} particle_t;

class Camera;

class Quake2Module {
public:
	Quake2Module();
	~Quake2Module();

	void GenerateLightMapLights(World* world, const std::string& LightMap, int YMin, int YMax);
	void DrawParticles(int num_particles, const particle_t particles[], const unsigned colortable[768], Camera* camera);
};

