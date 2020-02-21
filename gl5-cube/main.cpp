#include "sys.h"
#include "vec.h"
#include "shape.h"
#include "shapeprimitive.h"
#include "shader.h"


////////////////////////////////////////////////////////////////////////////////
class	Model
////////////////////////////////////////////////////////////////////////////////
{
public:
Shape	shape;
Shader	shader;
mat4	matProj; 
mat4	matModel; 
//------------------------------------------------------------------------------
Model( const char* fn, const ShapePrimitive::TYPE type, const vec3& pos, const vec3 scale ) : shape(ShapePrimitive(type)), shader(fn)
//------------------------------------------------------------------------------
{
	matModel.Identity();
	matModel.Scale( scale );
	matModel.Tlanslate( pos );

	matProj.Identity();

/*
	{
		float	mat[] = 
		{
			 1.0f, 0.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 1.0f, 0.0f,
			 0.0f, 0.0f,-22.0f, 1.0f,
		};
		for ( int i = 0 ; i < 16 ; i++ )
		{
			P.m_array[i] = mat[i] ;
		}
	}
*/


}
//------------------------------------------------------------------------------
void DrawModel()
//------------------------------------------------------------------------------
{
	shader.DrawPN( shape, matModel, matProj );
}

};






//------------------------------------------------------------------------------
int main()
//------------------------------------------------------------------------------
{

	Model sphere1( "s_PN.glsl", ShapePrimitive::SPHERE, vec3( 0, 0 , 0 ), vec3( 1.0, 1.0 , 1.0 ) );
	Model sphere2( "s_PN.glsl", ShapePrimitive::SPHERE, vec3( 1, 0 , 0 ), vec3( 1.0, 1.0 , 1.0 ) );
	Model   cube1( "s_PN.glsl", ShapePrimitive::CUBE  , vec3( 1.0, -1.0 , 0 ), vec3( 0.3, 1.0 , 0.3 ) );
	Model   cube2( "s_PN.glsl", ShapePrimitive::CUBE  , vec3( -1.2,-1.5 , 0 ), vec3( 0.3, 1.0 , 0.3 ) );
	Model   cube3( "s_PN.glsl", ShapePrimitive::CUBE  , vec3( 1.2,  1.5 , 0 ), vec3( 0.3, 1.0 , 0.3 ) );
	Model   cube4( "s_PN.glsl", ShapePrimitive::CUBE  , vec3( -1.2, 1.5 , 0 ), vec3( 0.3, 1.0 , 0.3 ) );

mat4&	matModel = cube1.matModel;
vec4	Position = vec4(	-1.0f, -0.0f, -1.0f, 1.0f );
{

	vec3	V = vec3(Position*matModel);
	vec3	P = vec3(0,0,0);
	vec3	N = vec3(0,0,-1);
	vec3	E = normalize( V-P );
	float	d = 4;
	float	t = -d/dot(E,N);
	vec3	Q = E*t + P;
printf("t %f\n", t );

matModel.print();
Position.print("Position\n");
V.print("V");
E.print("E");
Q.print("Q");


	Q.z = 1.0;

	float	z = V.z+4;
#if 1

	Q.x = V.x/z;
	Q.y = V.y/z;
	Q.z = 1.00;

#endif

//	gl_Position = vec4(Q,1);
Q.print("Q");
}

	// Depth
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL );	

	while ( sys.Update(1.0/60.0) )
	{

		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace( GL_CW );


		cube1.DrawModel();
		cube2.DrawModel();
		cube3.DrawModel();
		cube4.DrawModel();
		sphere1.DrawModel();
		sphere2.DrawModel();

	}

	return( 0 );
}



