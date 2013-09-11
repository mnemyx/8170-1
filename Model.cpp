/*
  Model.cpp

  Source File for Geometric Model Class
  Provides for construction of cuboid, cylinder and cone shapes tiled by triangles
  
  BIHE Computer Graphics    Donald H. House     6/22/06
  Modified - Gina Guerrero - Fall 2013 
*/

#include "Model.h"
#include <cstdlib>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

//
// Bookkeeping, remove all vertices and triangles
//
void Model::Clean(){
  nvertices = ntriangles = onvertices = 0;
}

//
// Insert a vertex into the vertex table
//
int Model::AddVertex(const Vector3d &v){
  if(nvertices == MAXVERTICES){
    cerr << "Number of vertices exceeds maximum of " << MAXVERTICES << endl;
    exit(1);
  }
  
  vertices[nvertices] = v;
  
  return nvertices++;
}


//
// Insert a vertex into the original vertex table
// Added 8/2013: GBG for CPSC8170 Proj 1
//
int Model::AddOVertex(const Vector3d &v){
  if(onvertices == MAXVERTICES){
    cerr << "Number of vertices exceeds maximum of " << MAXVERTICES << endl;
    exit(1);
  }
  
  overtices[onvertices] = v;
  
  return onvertices++;
}


//
// Insert a triangle and its normal into the triangle tables
//
int Model::AddTriangle(int v0, int v1, int v2){
  if(ntriangles == MAXTRIANGLES){
    cerr << "Number of triangles exceeds maximum of " << MAXTRIANGLES << endl;
    exit(1);
  }
  
  triangles[ntriangles][0] = v0;
  triangles[ntriangles][1] = v1;
  triangles[ntriangles][2] = v2;
  
  Vector3d V0(vertices[v0].x, vertices[v0].y, vertices[v0].z);
  Vector3d V1(vertices[v1].x, vertices[v1].y, vertices[v1].z);
  Vector3d V2(vertices[v2].x, vertices[v2].y, vertices[v2].z);
  
  Vector3d V01 = V1 - V0;
  Vector3d V02 = V2 - V0;
  normals[ntriangles] = (V01 % V02).normalize();
  
  return ntriangles++;
}


//
// Subdivides isohedron for sphere
// Added 8/2013: GBG for CPSC8170 Proj 1
//
void Model::Subdivide(int triIdx, int d, float r, Vector3d center) {

  if (d == 0) { return; }
  
  // I need to find the 3 vertices from the triangle I need...
  int v0 = triangles[triIdx][0]; 
  int v1 = triangles[triIdx][1];
  int v2 = triangles[triIdx][2];
  int v01, v12, v20, ov01, ov12, ov20;
  int nt0, nt1, nt2, nt3;
  double l0, l1, l2;
  Vector3d V0, V1, V2;
  
  V0.set(overtices[v0].x, overtices[v0].y, overtices[v0].z);
  V1.set(overtices[v1].x, overtices[v1].y, overtices[v1].z);
  V2.set(overtices[v2].x, overtices[v2].y, overtices[v2].z);
 
  Vector3d V01((V0.x + V1.x)/2.0, (V0.y + V1.y)/2.0, (V0.z + V1.z)/2.0);
  Vector3d V12((V1.x + V2.x)/2.0, (V1.y + V2.y)/2.0, (V1.z + V2.z)/2.0);
  Vector3d V20((V2.x + V0.x)/2.0, (V2.y + V0.y)/2.0, (V2.z + V0.z)/2.0);

  ov01 = AddOVertex(V01);
  ov12 = AddOVertex(V12);
  ov20 = AddOVertex(V20);
  
  V01 = V01.normalize();
  V12 = V12.normalize();
  V20 = V20.normalize();

  // need to multiply by radius?
  V01 = V01 * r ;
  V12 = V12 * r;
  V20 = V20 * r;
  
  // need to translate accordingly for center...
  V01 = V01 + center;
  V12 = V12 + center;
  V20 = V20 + center;
  
  // need to add the new vertices...? (could totally be uh, optimized by not storing the same points...but my head hurts)
  v01 = AddVertex(V01);
  v12 = AddVertex(V12);
  v20 = AddVertex(V20);
  
  // add triangles....?  4 of them...
  nt0 = AddTriangle(v0, v01, v20);
  nt1 = AddTriangle(v1, v12, v01);
  nt2 = AddTriangle(v2, v20, v12);
  nt3 = AddTriangle(v01, v12, v20);
  
  // recurse....oh god don't break...do I need to do this? sigh..
  Subdivide(nt0, d-1, r, center);
  Subdivide(nt1, d-1, r, center);
  Subdivide(nt2, d-1, r, center);
  Subdivide(nt3, d-1, r, center);  
  
  return;
}

//
// Constructor, make sure model is empty
//
Model::Model(){
  Clean();
}

//
// Make a isohedron for sphere
// Added 8/2013: GBG for CPSC8170 Proj 1
//
void Model::BuildSphere(float r, int depth, double x, double y, double z) {
  
  int i, j;
  int isign, jsign, ksign;
  int tsign = -1;
  Vector3d vector;
  Vector3d center(x,y,z);
  int v[12];
  double l;
  
  double t = (1.0 + sqrt(5.0))/2.0;
  int vlist[60] = {0,11,5,   0,5,1,   0,1,7,   0,7,10,   0,10,11,   //sets of vertices
                   1,5,9,   5,11,4,   11,10,2,   10,7,6,   7,1,8,
                   3,9,4,   3,4,2,   3,2,6,   3,6,8,   3,8,9,
                   4,9,5,   2,4,11,   6,2,10,   8,6,7,   9,8,1};

  // delete any old data that may have been built previously
  Clean();  		
  
  j = 0;
  // construct the 12 vertices
   for (i = 0; i < 3; i++) 
     for(jsign = -1; jsign <= 1; jsign += 2)
       for(isign = -1; isign <= 1; isign += 2) {
          switch(i) {
            case(0): vector.set(1 * isign, -t * jsign, 0); break;
            case(1): vector.set(0, 1 * isign, -t * jsign); break;
            case(2): vector.set(-t * jsign, 0, 1 * isign); break;
          }
	  
	  // remember the original points for the isohedron
	  AddOVertex(vector);
	  
	  // puts the point in the unit circle & multiplies it to radius
	  vector = vector.normalize();
	  vector = vector * r;
	  
	  // need to consider the center...
	  vector = vector + center;
	  
	  v[j++] = AddVertex(vector);
   }
   
     
  // add the 20 triangles for the main faces
  for(i = 0; i < 60; i += 3)
    AddTriangle(v[vlist[i]], v[vlist[i + 1]], v[vlist[i + 2]]);
    
  // refine the faces
  for(i = 0; i < 20; i++) 
    Subdivide(i, depth, r, center);
}

//
// Make a cuboid model
//
void Model::BuildCuboid(float width, float height, float depth, double x, double y, double z){
  int v[8];
  Vector3d vector;
  Vector3d center(x,y,z);
  int i;
  int isign, jsign, ksign;
  int vlist[36] = {0, 2, 3,     0, 3, 1,    // back
                   4, 5, 6,     5, 7, 6,    // front
		   0, 4, 2,     2, 4, 6,    // left
		   1, 7, 5,     1, 3, 7,    // right
		   0, 1, 4,     1, 5, 4,    // bottom
		   2, 7, 3,     2, 6, 7};   // top
		   
  // delete any old data that may have been built previously
  Clean();
  
  // construct the 8 vertices for the cubeoid.
  i = 0;
  for(ksign = -1; ksign <= 1; ksign += 2)
    for(jsign = -1; jsign <= 1; jsign += 2)
      for(isign = -1; isign <= 1; isign += 2){
		vector.set(isign * width / 2, jsign * height / 2, ksign * depth / 2);
	    vector = vector + center;

	    v[i++] = AddVertex(vector);
      }
	
  // construct the 12 triangles that make the 6 faces
  for(i = 0; i < 36; i += 3)
    AddTriangle(v[vlist[i]], v[vlist[i + 1]], v[vlist[i + 2]]);
}


//
// Make a plane
// Added 9/2013 - Proj 1 - GBG
//
void Model::BuildPlane(float l, float h, int orientation, double x, double y, double z) {
  int v[4];
  Vector3d vector;
  Vector3d center(x,y,z);
  int i;
  int isign, jsign;
  int vlist[6] = {0, 2, 1,     1, 2, 3};   // 2 triangles
  
  // delete any old data that may have been built previously
  Clean();
  
  // construct the 8 vertices for the cubeoid.
  i = 0;
	for(jsign = -1; jsign <= 1; jsign += 2)
	  for(isign = -1; isign <= 1; isign += 2){
		switch(orientation) {
			case(FRONTBACK):
			    if( z < 0 )
					vector.set(jsign * l / 2, isign * h / 2, 0);
				else
					vector.set(isign * l / 2, jsign * h / 2, 0); break;
			case(SIDES): 
				if( x < 0 )
					vector.set(0, jsign * l / 2, isign * h / 2);
				else
					vector.set(0, isign * l / 2, jsign * h / 2); break;
			case(TOPBOTTOM):
				if ( y > 0 )
					vector.set(jsign * l / 2, 0, isign * h / 2); 
				else
					vector.set(isign * l / 2, 0, jsign * h / 2); break;
		}
		
		vector = vector + center;
		
		// normals are wrong...how to fix it?
		v[i++] = AddVertex(vector);
	  }
	
  // construct the 12 triangles that make the 6 faces
  for(i = 0; i < 6; i += 3)
    AddTriangle(v[vlist[i]], v[vlist[i + 1]], v[vlist[i + 2]]);
}


//
// Draw the current model in wireframe or shaded
//
void Model::Draw(const float* color){
  int itri, ivertex;
  
  glColor4f(color[0], color[1], color[2], color[3]); 
  for(itri = 0; itri < ntriangles; itri++){
    glBegin(GL_TRIANGLES);
   
	glNormal3f(normals[itri].x, normals[itri].y, normals[itri].z);
	
      for(int i = 0; i < 3; i++){
		ivertex = triangles[itri][i];
		glVertex3f(vertices[ivertex].x, vertices[ivertex].y, vertices[ivertex].z);
      }
    glEnd();
  }
}

//
// Draw but specifically for the plane - to make them 2 solid colors.
//
void Model::Draw(const float* frontC, const float* backC){
  int itri, ivertex, i;
  
  glColor4f(backC[0], backC[1], backC[2], backC[3]); 
  //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for(itri = 0; itri < ntriangles; itri++){

	glBegin(GL_TRIANGLES);
	
	glNormal3f(normals[itri].x, normals[itri].y, normals[itri].z);
	
    for(int i = 0; i < 3; i++){
	  ivertex = triangles[itri][i];
	  glVertex3f(vertices[ivertex].x, vertices[ivertex].y, vertices[ivertex].z);
    }
		
    glEnd();
  }

  glColor4f(frontC[0], frontC[1], frontC[2], backC[3]);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for(itri = 0; itri < ntriangles; itri++){
	glBegin(GL_TRIANGLES);
	
	glNormal3f(normals[itri].x, normals[itri].y, normals[itri].z);
		
    for(int i = 0; i < 3; i++){
	  ivertex = triangles[itri][i];
	  glVertex3f(vertices[ivertex].x, vertices[ivertex].y, vertices[ivertex].z);
    }
		
    glEnd();
  }
}