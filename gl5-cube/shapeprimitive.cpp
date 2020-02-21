#include	"sys.h"
#include	"shape.h"
#include	"shapeprimitive.h"

//------------------------------------------------------------------------------
ShapePrimitive::ShapePrimitive( TYPE type )
//------------------------------------------------------------------------------
{
	switch( type )
	{
	case CUBE:
		CreateCube();
		break;
	case SPHERE:
		CreateSphere();
		break;
	
	}
}

//------------------------------------------------------------------------------
void ShapePrimitive::CreateSphere()
//------------------------------------------------------------------------------
{
	struct	Position	{	float	x,y,z;	};
	struct	Normal		{	float	x,y,z;	};

	int	n = 10;
	int	sv = n*5;
	int	sh = n*10;
	int	cntVertex = sv*sh;
	{
		float	r = 1.0;

	  	Position*	tblPosition = (Position*)malloc( cntVertex*sizeof(Position) );
		Normal*		tblNormal = (Normal*)malloc( cntVertex*sizeof(Normal) );

		for ( int v = 0 ; v < sv ; v++ )
		{
			float th = (float)v/(float)(sv-1)*M_PI;
			float y = r*cos(th);
			float l = r*sin(th);
			for ( int h = 0 ; h < sh ; h++ )
			{
				float th = (float)h/(float)(sh-1)*2*M_PI;
				float x = l*sin(th);
				float z = l*cos(th);

				int	j = v*sh+h;
				tblPosition[j].x = x;
				tblPosition[j].y = y;
				tblPosition[j].z = z;
				float l = sqrt( x*x + y*y + z*z );
				tblNormal[j].x = x/l;
				tblNormal[j].y = y/l;
				tblNormal[j].z = z/l;
			}
		}

		m_cntIndex = (sv-1)*sh*2;
	
		glGenBuffers( 1, &m_hdlVertex_Position );
		glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Position );
		glBufferData(GL_ARRAY_BUFFER, cntVertex*sizeof(Position), tblPosition, GL_STATIC_DRAW  );

		glGenBuffers( 1, &m_hdlVertex_Normal );
		glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Normal );
		glBufferData(GL_ARRAY_BUFFER, cntVertex*sizeof(Normal), tblNormal, GL_STATIC_DRAW  );

		free( tblPosition );
	}


	{
		int*	tblIndex = (int*)malloc( m_cntIndex*sizeof(int) );
		int	i = 0;
		for ( int v = 0 ; v < sv-1 ; v++ )
		{
			for ( int h = 0 ; h < sh ; h++ )
			{
				int	j = v*sh+h;
				tblIndex[i++] = j;
				tblIndex[i++] = j+sh;
			}
		}
		
		glGenBuffers(1, &m_hdlVertex_Index );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_cntIndex*sizeof(int), tblIndex, GL_STATIC_DRAW);

		free( tblIndex );
	}

}

//------------------------------------------------------------------------------
void ShapePrimitive::CreateCube()
//------------------------------------------------------------------------------
{

	int	tblIndex[] =
	{
		   0,   1,   2,   3,	  3,  4+1,
		 4+1, 4+0, 4+3, 4+2,    4+2,  8+0,
		 8+0, 8+1, 8+2, 8+3,	8+3, 12+1,
		12+1,12+0,12+3,12+2,   12+2, 16+0,
		16+0,16+1,16+2,16+3,   16+3, 20+1,
		20+1,20+0,20+3,20+2,

	};
	glGenBuffers(1, &m_hdlVertex_Index );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hdlVertex_Index );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tblIndex), tblIndex, GL_STATIC_DRAW);

	m_cntIndex = sizeof(tblIndex)/sizeof(int);


	float	tblPosition[] =
	{
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,

		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,

	};
	glGenBuffers( 1, &m_hdlVertex_Position );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Position );
	glBufferData(GL_ARRAY_BUFFER, sizeof(tblPosition), tblPosition, GL_STATIC_DRAW  );

	float	tblNormal[] =
	{
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,

		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,

		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,

		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,

		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,

		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
	};
	glGenBuffers( 1, &m_hdlVertex_Normal );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Normal );
	glBufferData(GL_ARRAY_BUFFER, sizeof(tblNormal), tblNormal, GL_STATIC_DRAW  );

	float	tblUv[] =
	{
		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,

		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,

		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,

		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,

		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,

		 0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,
	};
	glGenBuffers( 1, &m_hdlVertex_Uv );
	glBindBuffer(GL_ARRAY_BUFFER, m_hdlVertex_Uv );
	glBufferData(GL_ARRAY_BUFFER, sizeof(tblUv), tblUv, GL_STATIC_DRAW  );


}
