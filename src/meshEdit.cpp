#include <float.h>
#include <assert.h>
#include "meshEdit.h"
#include "mutablePriorityQueue.h"
#include "error_dialog.h"

namespace CMU462 {

VertexIter HalfedgeMesh::splitEdge(EdgeIter e0) {
	// text command: s
	//
	// TODO: (meshEdit)
	// This method should split the given edge and return an iterator to the
	// newly inserted vertex. The halfedge of this vertex should point along
	// the edge that was split, rather than the new edges.

  	//check if both sides are triangles
	//cout << "test" << endl;

	/*
	See: http://462cmu.github.io/asst2_meshedit/
	GUIDE TO IMPLEMENTING EDGE OPERATIONS
	ON A HALFEDGE DATA STRUCTURE
	CMU CS 15-462 (FALL 2015)

   v3            h9                    v1  
               <-------
      o---------------e4-------------o 
      |        ------->            / |
      |           h5             /   |
      |    f1                  /     |
      |                       /      |
      | ^              .    /        |
      | |h4           .   /          |
      | |         h3.   /  ^         |  ^ 
      |	e3       .   e0 .           |  |h6
  h8| |          v  /   . h0         e1 |        
    v |           /    .           | |      
      |         /               h1 | |     
      |       /                    v |   
      |     /              f0        |
      |   /           h2             | 
      | /         <------            | 
      o--------------e2--------------o    
                  ------>  
   v0                h7                v2   
   -----------------------------------------------------
ahha,

   -----------------------------------------------------
   After split (see notes/EdgeSplit.pdf for a up to date picture)


	*/    

   // //Ignore requests to split boundary edges for now
   // //   (just return immediately if either neighboring face is a boundary loop).
   // if (e0->isBoundary()) {
   //    return e0->halfedge()->vertex();
   // }

   // splitting can work on a boundary:
	if (e0->isBoundary() && e0->halfedge()->face()->degree() != 3) {return e0->halfedge()->vertex(); }
	else if (e0->isBoundary() && e0->halfedge()->face()->degree() == 3) {

      //
      //---------------------------------
      //
      // we are on a boundary



      //-----------------------------------------------------
      // collect elements
      //
      // halfedges
      HalfedgeIter h0 = e0->halfedge();
      HalfedgeIter h1 = h0->next();
      HalfedgeIter h2 = h1->next();
      HalfedgeIter h3 = h0->twin();
      HalfedgeIter h6 = h1->twin();
      HalfedgeIter h7 = h2->twin();
      //
      // vertices 
      VertexIter v0 = h0->vertex();
      VertexIter v1 = h1->vertex();
      VertexIter v2 = h2->vertex();
      //
      // edges
      //e0 = h0->edge(); passed in.  do not redeclare
      EdgeIter e1 = h1->edge();
      EdgeIter e2 = h2->edge();
      //
      // faces (pick the only unambiguous elements)
      FaceIter f0 = h0->face();
      
      //-----------------------------------------------------
      // new elements
      //

      //1 new vertex
      VertexIter v4 = newVertex();  //HalfedgeMesh.newVertex();
      v4->position = e0->centroid();

      //initialize new edge, face and halfedge
      //2 new edges 
      EdgeIter e5 = newEdge();
      EdgeIter e7 = newEdge();

      //1 new face (we had 1 since we are on a boundary)
      FaceIter f2 = newFace();

      // 4 new halfedges (2 for each new edge)
      HalfedgeIter h10 = newHalfedge();
      HalfedgeIter h11 = newHalfedge();
      HalfedgeIter h14 = newHalfedge();
      HalfedgeIter h15 = newHalfedge();


      //  Now make new connectivity

      //assign value to new elems
      v4->halfedge() = h0;
      e5->halfedge() = h11; // e5->halfedge = new_h[1] aka h10
      e7->halfedge() = h14; // e7->halfedge = new_h[3] aka h14

      f2->halfedge() = h2; //h11; //h2; // f2->halfedge = new_h[1] = h11

      h10->setNeighbors(h3->next(), h11, v4, e5, f2); //f3);
      h11->setNeighbors(h14, h10, v0, e5, f2);
      h14->setNeighbors(h2, h15, v4, e7, f2);
      h15->setNeighbors(h0, h14, v2, e7, f0);


      e0->halfedge() = h0;
      f0->halfedge() = h1;
      //f1->halfedge() = h5;
      h0->setNeighbors(h0->next(), h0->twin(), v4, h0->edge(), h0->face());
      h1->setNeighbors(h15, h1->twin(), h1->vertex(), h1->edge(), h1->face());
      h2->setNeighbors(h11, h2->twin(), h2->vertex(), h2->edge(), f2);
      h3->setNeighbors(h10, h3->twin(), h3->vertex(), h3->edge(), h3->face());
      //h4->setNeighbors(h12, h4->twin(), h4->vertex(), h4->edge(), f3);

      /*
      void setNeighbors(HalfedgeIter next, 
                        HalfedgeIter twin, 
                        VertexIter vertex,
                        EdgeIter edge, 
                        FaceIter face) {
      */

      return v4;
   }
   // not a tri mesh?:
   if (e0->halfedge()->face()->degree() != 3 && e0->halfedge()->twin()->face()->degree() != 3) {
      return e0->halfedge()->vertex();
   }
   //
   //---------------------------------
   //
   // now we are on a standard face 


   //-----------------------------------------------------
	// collect elements
	//
	// halfedges
	HalfedgeIter h0 = e0->halfedge();
	HalfedgeIter h1 = h0->next();
	HalfedgeIter h2 = h1->next();
	HalfedgeIter h3 = h0->twin();
	HalfedgeIter h4 = h3->next();
	HalfedgeIter h5 = h4->next();
	HalfedgeIter h6 = h1->twin();
	HalfedgeIter h7 = h2->twin();
	HalfedgeIter h8 = h4->twin();
	HalfedgeIter h9 = h5->twin();
	//
	// vertices (always pull from the interior)
	VertexIter v0 = h0->vertex();
	VertexIter v1 = h3->vertex();
	VertexIter v2 = h2->vertex();
	VertexIter v3 = h5->vertex();
	//
	// edges
	//e0 = h0->edge(); passed in.  do not redeclare
	EdgeIter e1 = h1->edge();
	EdgeIter e2 = h2->edge();
	EdgeIter e3 = h4->edge();
	EdgeIter e4 = h5->edge();
	//
	// faces (pick the only unambiguous elements)
	FaceIter f0 = h0->face();
	FaceIter f1 = h3->face();
   //-----------------------------------------------------
	// new elements
	//

   //1 new vertex
   VertexIter v4 = newVertex();  //HalfedgeMesh.newVertex();
   v4->position = e0->centroid();


	//initialize new edge, face and halfedge vectors
	// vector<EdgeIter> new_e;
	// vector<FaceIter> new_f;
	// vector<HalfedgeIter> new_h;

	// //3 new edges 
	// new_e.push_back(newEdge());  //new_e[0] = e5
	// new_e.push_back(newEdge());  //new_e[1] = e6
	// new_e.push_back(newEdge());  //new_e[2] = e7

	// //2 new faces (we already had f0 and f1) now we add f2 and f3
	// new_f.push_back(newFace());   //new_f[0] = f2
	// new_f.push_back(newFace());   //new_f[1] = f3

	// //6 new halfedges (2 for each new edge)
	// for (size_t i = 0; i < 6; i++) {
	// 	new_h.push_back(newHalfedge());
	// }
   /*{
      new_h[0] : h10
      new_h[1] : h11
      new_h[2] : h12
      new_h[3] : h13
      new_h[4] : h14
      new_h[5] : h15
   }
   */

   //initialize new edge, face and halfedge
   //3 new edges 
   EdgeIter e5 = newEdge();
   EdgeIter e6 = newEdge();
   EdgeIter e7 = newEdge();

   //2 new faces (we already had 2)
   FaceIter f2 = newFace();
   FaceIter f3 = newFace();

   //6 new halfedges (2 for each new edge)
   HalfedgeIter h10 = newHalfedge();
   HalfedgeIter h11 = newHalfedge();
   HalfedgeIter h12 = newHalfedge();
   HalfedgeIter h13 = newHalfedge();
   HalfedgeIter h14 = newHalfedge();
   HalfedgeIter h15 = newHalfedge();

   //assign value to new elems
   // v4->halfedge() = h0;
   // new_e[0]->halfedge() = new_h[0]; // e5->halfedge = new_h[1] aka h10
   // new_e[1]->halfedge() = new_h[3]; // e6->halfedge = new_h[4] aka h13
   // new_e[2]->halfedge() = new_h[4]; // e7->halfedge = new_h[3] aka h14

   // new_f[0]->halfedge() = new_h[1]; // f2->halfedge = new_h[1] = h11
   // new_f[1]->halfedge() = new_h[0]; // f3->halfedge = new_h[0] = h10

   // new_h[0]->setNeighbors(h4, new_h[1], v4, new_e[0], new_f[1]);
   // new_h[1]->setNeighbors(new_h[4], new_h[10], v0, new_e[0], new_f[0]);
   // new_h[2]->setNeighbors(new_h[0], new_h[3], v3, new_e[1], new_f[1]);
   // new_h[3]->setNeighbors(h5, new_h[2], v4, new_e[1], f1);
   // new_h[4]->setNeighbors(h2, new_h[5], v4, new_e[2], new_f[0]);
   // new_h[5]->setNeighbors(h0, new_h[4], v4, new_e[2], f0);

   //assign value to new elems
   v4->halfedge() = h0;
   e5->halfedge() = h10; // e5->halfedge = new_h[1] aka h10
   e6->halfedge() = h13; // e6->halfedge = new_h[4] aka h13
   e7->halfedge() = h14; // e7->halfedge = new_h[3] aka h14

   f2->halfedge() = h2; //h11; //h2; // f2->halfedge = new_h[1] = h11
   f3->halfedge() = h4; //h10; //h4; // f3->halfedge = new_h[0] = h10

   h10->setNeighbors(h4, h11, v4, e5, f3);
   h11->setNeighbors(h14, h10, v0, e5, f2);
   h12->setNeighbors(h10, h13, v3, e6, f3);
   h13->setNeighbors(h5, h12, v4, e6, f1);
   h14->setNeighbors(h2, h15, v4, e7, f2);
   h15->setNeighbors(h0, h14, v2, e7, f0);


   //reassign value to old elems
   e0->halfedge() = h0;
   f0->halfedge() = h1;
   f1->halfedge() = h5;
   h0->setNeighbors(h0->next(), h0->twin(), v4, h0->edge(), h0->face());
   h1->setNeighbors(h15, h1->twin(), h1->vertex(), h1->edge(), h1->face());
   h2->setNeighbors(h11, h2->twin(), h2->vertex(), h2->edge(), f2);
   h3->setNeighbors(h13, h3->twin(), h3->vertex(), h3->edge(), h3->face());
   h4->setNeighbors(h12, h4->twin(), h4->vertex(), h4->edge(), f3);
   

   return v4;
}


VertexIter HalfedgeMesh::collapseEdge_new(EdgeIter e0) {
   // working from EdgeCollapse.svg (incomplete drawing, but names match EdgeSplit.pdf)

   // collapse on boundary is not implemented
	if (e->isBoundary()) { return e0->halfedge()->vertex(); }


	HalfedgeIter h0 = e->halfedge(); 
	HalfedgeIter old_h = h0; // h might change after we delete halfedge from triangles
	HalfedgeIter h3 = h->twin();

   FaceIter f0 = h0.face();
   FaceIter f1 = h3.face();


	//use faces to check if edge is in triangle
	bool right = false;
	bool left  = false;	
	if (f0->degree() == 3) { right = true; }
	if (f1->degree() == 3) { left  = true; }

	//set the center vertex
	VertexIter v0  = h0->vertex();
	VertexIter v1  = h3->vertex();
	v0->position   = e->centroid();
   // pull in the halfedges
	v0->halfedge() = h3->next()->twin()->next(); // move h0 to the first existing similar place on a triangle "above"
	v1->halfedge() = h0->next()->twin()->next(); // move h3 to the first existing similar place on a triangel "below"

   // // cache local "up"
	// //   edges, faces and halfedges 
	// vector<EdgeIter> up_e;
	// vector<FaceIter> up_f;
	// vector<HalfedgeIter> up_h;
	// //vector<VertexIter> up_v;
   // while () {

   // }

   // // cache local "down"
	// //   edges, faces and halfedges 
	// vector<EdgeIter> down_e;
	// vector<FaceIter> down_f;
	// vector<HalfedgeIter> down_h;
	// //vector<VertexIter> up_v;


	if (right) {
		if (h->next()->isBoundary() || h->next()->next()->isBoundary()) {
			return e->halfedge()->vertex();
		}
		//get related halfedges
      HalfedgeIter r1 = h0->next();
      HalfedgeIter r2 = r1->next();
		HalfedgeIter r3 = r2->twin();
		HalfedgeIter r4 = r1->twin();
		//get related edge
		EdgeIter er1 = r2->edge();
		EdgeIter er2 = r1->edge();
		//get related vertex
		VertexIter v1r = r2->vertex();

		//reassign value
		v1r->halfedge() = r3->next();
		r3->twin() = r4;
		r4->twin() = r3;
		r4->edge() = er1;
		er1->halfedge() = r3;

		h0 = r4->next();

		deleteHalfedge(r1);
		deleteHalfedge(r2);
		deleteEdge(er2);
   }
   else {
      // get related halfedges
      HalfedgeIter r1 = h0->next();
      HalfedgeIter r2 = h0;
      while (r2->next() != h0 ) {
         r2 = r2->next(); // go around the boundary
      }
      //reassign value
      r2->next() = r1;
      r1->vertex() = v1;
      r1->face->halfedge() = r1; // needed?

      h0 = r1->twin()->next();
   }

	//update the bottom vertex to the returned vertex
	//HalfedgeIter temp1 = h->next()->twin()->next();
	cout << "num of edges; " << h0->vertex()->degree() << endl;
	HalfedgeIter temp1 = h0;
	while (temp1 != h3) {
      temp1->vertex() = v;
      temp1 = temp1->twin()->next();

   return VertexIter v4;
}


VertexIter HalfedgeMesh::collapseEdge(EdgeIter e) {
  // command None
  //
  // TODO: (meshEdit)
  // This method should collapse the given edge and return an iterator to
  // the new vertex created by the collapse.
	if (e->isBoundary()) { return e->halfedge()->vertex(); }

	HalfedgeIter h = e->halfedge();
	HalfedgeIter old_h = h; // h might change after we delete halfedge from triagnles
	HalfedgeIter h0 = h->twin();
	FaceIter f0 = h->face();
	FaceIter f1 = h0->face();
	//check if edge is in triangle
	bool right = false;
	bool left = false;	
	if (f0->degree() == 3) { right = true; }
	if (f1->degree() == 3) { left = true; }

	//set the center vertex
	VertexIter v = h->vertex();
	VertexIter v0 = h0->vertex();
	v->position = e->centroid();
	v->halfedge()= h0->next()->twin()->next();
	v0->halfedge() = h->next()->twin()->next();

	if (right) {
		if (h->next()->isBoundary() || h->next()->next()->isBoundary()) {
			return e->halfedge()->vertex();
		}
		//get related halfedges
		HalfedgeIter r1 = h->next();
		HalfedgeIter r2 = r1->next();
		HalfedgeIter r3 = r2->twin();
		HalfedgeIter r4 = r1->twin();
		//get related edge
		EdgeIter er1 = r2->edge();
		EdgeIter er2 = r1->edge();
		//get related vertex
		VertexIter v1 = r2->vertex();

		//reassign value
		v1->halfedge() = r3->next();
		r3->twin() = r4;
		r4->twin() = r3;
		r4->edge() = er1;
		//r3->edge() = er1; // redundant
		er1->halfedge() = r3;

		h = r4->next();

		deleteHalfedge(r1);
		deleteHalfedge(r2);
		deleteEdge(er2);
	}
	else {
		//get related halfedges
		HalfedgeIter r1 = h->next();
		HalfedgeIter r2 = h;
		while (r2->next() != h) {
			r2 = r2->next();
		}
		//reassign value
		r2->next() = r1;
		r1->vertex() = v;
		//r1->face()->halfedge() = r1; //needed?

		//h = r1->twin()->next(); // inconsistent with earlier case, but does not hurt?
      h = r1;
	}
	
	//update the bottom vertex to the returned vertex
	//HalfedgeIter temp1 = h->next()->twin()->next();
	cout << "num of edges; " << h->vertex()->degree() << endl;
	HalfedgeIter temp1 = h;	
	while (temp1 != h0) {
		temp1->vertex() = v;
		temp1 = temp1->twin()->next(); // set temp1 to halfedge just "before" h
	}
	cout << "collapse 1" << endl;
	if (left) {
		if (h0->next()->isBoundary() || h0->next()->next()->isBoundary()) {
			return e->halfedge()->vertex();
		}
		//get related halfedges
		HalfedgeIter l1 = h0->next();
		HalfedgeIter l2 = l1->next();
		HalfedgeIter l3 = l2->twin();
		HalfedgeIter l4 = l1->twin();
		//get related edge
		EdgeIter el1 = l1->edge();
		EdgeIter el2 = l2->edge();
		//get related vertex
		VertexIter v2 = l2->vertex();

		//reassign value
		v2->halfedge() = l3->next();
		l3->twin() = l4;
		l4->twin() = l3;
		l3->edge() = el1;
		el1->halfedge() = l4;
		//l3->vertex() = v;
		deleteHalfedge(l1);
		deleteHalfedge(l2);
		deleteEdge(el2);
	}
	else {
		//get related halfedges
		HalfedgeIter l1 = h0->next();
		HalfedgeIter l2 = h0;
		while (l2->next() != h0) {
			l2 = l2->next();
		}
		//reassign value
		l2->next() = l1;
		l1->face()->halfedge() = l1;
	}
	//deleteHalfedge(h);
	deleteHalfedge(old_h);
	deleteHalfedge(h0);
	deleteEdge(e);
	deleteVertex(v0);
	if (right) { deleteFace(f0); }
	if (left) { deleteFace(f1); }
	cout << "collapse complete" << endl;
	return v;
}


VertexIter HalfedgeMesh::collapseFace(FaceIter f) {
  // TODO: (meshEdit)
  // This method should collapse the given face and return an iterator to
  // the new vertex created by the collapse.
  showError("collapseFace() not implemented.");
  return VertexIter();
}

FaceIter HalfedgeMesh::eraseVertex(VertexIter v) {
  // TODO: (meshEdit)
  // This method should replace the given vertex and all its neighboring
  // edges and faces with a single face, returning the new face.

  return FaceIter();
}

FaceIter HalfedgeMesh::eraseEdge(EdgeIter e) {
  // TODO: (meshEdit)
  // This method should erase the given edge and return an iterator to the
  // merged face.

  showError("eraseVertex() not implemented.");
  return FaceIter();
}


EdgeIter HalfedgeMesh::flipEdge(EdgeIter e0) {
  // TODO: (meshEdit)
  // This method should flip the given edge and return an iterator to the
  // flipped edge.

  // Note: This does a complete reassignment of references, which will likely include
  // changing things that don't technically need to be changed (like anyHalfEdge refs
  // that weren't actually invalidated). This is done for simplicity and conciseness.

  //
  // NOTE: -for tris only
  //       -ignore any edge flip that would make the surface non-manifold
  //       -what about low degree vertices, or very small meshes like tetrahedron?

  // There is a guide for this code 
  //  todo: point at it here
  /*
	See: 
	http://15462.courses.cs.cmu.edu/fall2015content/misc/HalfedgeEdgeOpImplementationGuide.pdf
	and:
	http://462cmu.github.io/asst2_meshedit/
	GUIDE TO IMPLEMENTING EDGE OPERATIONS
	ON A HALFEDGE DATA STRUCTURE
	CMU CS 15-462 (FALL 2015)
    -----------------------------------------------------
    Before flip

		v3            h9                    v1  
			        <-------
			o---------------e4-------------o 
         |        ------->            . |
         |           h5             .   |
			|    f1                  .     |
			|                       .      |
         | ^              .    .        |
         | |h4           .   .          |
	    | |         h3.   .     ^        |  ^ 
		 |	e3          .   e0    .        |  |h6
     h8| |         v   .     . h0       e1 |        
	    v |           .     .          | |      
		   |         .               h1 | |     
			|       .                    v |   
			|     .               f0       |
			|   .           h2             | 
         | .         <------            | 
			o--------------e2--------------o    
				        ------>  
		v0                h7                v2   
    -----------------------------------------------------
	ahha,
	edge flip rotates the interior halfedge elements.
	easy once you see it!

	- halfedges rotate (keeping their orientation)
	  * a middle halfedge stays in the middle
	    so only middle twins will remain twins
	- edges stay the same
	- vertices stay the same
	- faces rotate with halfedges 
    -----------------------------------------------------
    After flip

		v3            h9                     v1  
			        <-------
			o---------------e4-------------o 
         | .      ------->              |
         |   .       h4                 |
			|     .             f1         |
			|      .                       |
         |        .   ^           h5|   |
         | ^        .   .  h3       |   |
         | |h2       e0   .         v   |  ^ 
		 |	| |       .    .   .           |  |h6
     h8| |           .   .              e1 |      
	    v	|         h0  .   .            |      
		  e3              v    .          |     
			|                     .        |   
			|     f0                 .     |
			|               h1         .   | 
         |           <------          . | 
			o--------------e2--------------o    
				        ------>  
		v0                h7                v2   

  */    

	if (e0->isBoundary()) {
		return e0;
	}

   //-----------------------------------------------------
	// collect elements
	//
	// halfedges
	HalfedgeIter h0 = e0->halfedge();
	HalfedgeIter h1 = h0->next();
	HalfedgeIter h2 = h1->next();
	HalfedgeIter h3 = h0->twin();
	HalfedgeIter h4 = h3->next();
	HalfedgeIter h5 = h4->next();
	HalfedgeIter h6 = h1->twin();
	HalfedgeIter h7 = h2->twin();
	HalfedgeIter h8 = h4->twin();
	HalfedgeIter h9 = h5->twin();
	//
	// vertices (always get vertices from the interior halfedges)
	VertexIter v0 = h0->vertex();
	VertexIter v1 = h3->vertex();
	VertexIter v2 = h2->vertex();
	VertexIter v3 = h5->vertex();
	//
	// edges
	//e0 = h0->edge(); passed in.  do not redeclare
	EdgeIter e1 = h1->edge();
	EdgeIter e2 = h2->edge();
	EdgeIter e3 = h4->edge();
	EdgeIter e4 = h5->edge();
	//
	// faces (pick the only unambiguous elements)
	FaceIter f0 = h0->face();
	FaceIter f1 = h3->face();


   //-----------------------------------------------------
	// Reassign elements
	//
	
	// halfedges

	// h0->next()   = h1;
	// h0->twin()   = h3;
	// h0->vertex() = v3; 
	// h0->edge()   = e0;
	// h0->face()   = f0;
	h0->setNeighbors(h1,h3,v3,e0,f0);
	h1->setNeighbors(h2,h7,v2,e2,f0);
	h2->setNeighbors(h0,h8,v0,e3,f0);
	h3->setNeighbors(h4,h0,v2,e0,f1);
	h4->setNeighbors(h5,h9,v3,e4,f1);
	h5->setNeighbors(h3,h6,v1,e1,f1);
	
	// outside elements

	// h6->next()   = h6->next(); //constant and unknown
	// h6->twin()   = h5;
	// h6->vertex() = v2; // constant
	// h6->edge()   = e1; // constant
	// h6->face()   = h6->face(); //constant and unknown
	h6->setNeighbors(h6->next(),h5,v2,e1,h6->face());
	h9->setNeighbors(h9->next(),h4,v1,e4,h9->face());
	h8->setNeighbors(h8->next(),h2,v3,e3,h8->face());
	h7->setNeighbors(h7->next(),h1,v0,e2,h7->face());

	// vertices 

	v0->halfedge() = h2; //this is now the only interior halfedge running away from v0
	v1->halfedge() = h5; // similar to v0. chose based on interior halfedge connectivity
	v2->halfedge() = h3; //change from h2 to h3 switched the face it is "in"
	v3->halfedge() = h0;// switched face it is "in" as well
	
   // remember, half edges contain all the connectivity
	// other elements are more  arbitrary
   
   // v2->halfedge() = h3; // still works in easy tests
	// v3->halfedge() = h1; // still works in easy tests

	// edges (sticking with interior) bad assumption

	e0->halfedge() = h0;
	e1->halfedge() = h5;
	e2->halfedge() = h1;
	e3->halfedge() = h2;
	e4->halfedge() = h4;

	// faces 
	f0->halfedge() = h0;
	f1->halfedge() = h3;


  return e0;
}


void HalfedgeMesh::subdivideQuad(bool useCatmullClark) {
  // Unlike the local mesh operations (like bevel or edge flip), we will perform
  // subdivision by splitting *all* faces into quads "simultaneously."  Rather
  // than operating directly on the halfedge data structure (which as you've
  // seen
  // is quite difficult to maintain!) we are going to do something a bit nicer:
  //
  //    1. Create a raw list of vertex positions and faces (rather than a full-
  //       blown halfedge mesh).
  //
  //    2. Build a new halfedge mesh from these lists, replacing the old one.
  //
  // Sometimes rebuilding a data structure from scratch is simpler (and even
  // more
  // efficient) than incrementally modifying the existing one.  These steps are
  // detailed below.

  // TODO Step I: Compute the vertex positions for the subdivided mesh.  Here
  // we're
  // going to do something a little bit strange: since we will have one vertex
  // in
  // the subdivided mesh for each vertex, edge, and face in the original mesh,
  // we
  // can nicely store the new vertex *positions* as attributes on vertices,
  // edges,
  // and faces of the original mesh.  These positions can then be conveniently
  // copied into the new, subdivided mesh.
  // [See subroutines for actual "TODO"s]
  if (useCatmullClark) {
    computeCatmullClarkPositions();
  } else {
    computeLinearSubdivisionPositions();
  }

  // TODO Step II: Assign a unique index (starting at 0) to each vertex, edge,
  // and
  // face in the original mesh.  These indices will be the indices of the
  // vertices
  // in the new (subdivided mesh).  They do not have to be assigned in any
  // particular
  // order, so long as no index is shared by more than one mesh element, and the
  // total number of indices is equal to V+E+F, i.e., the total number of
  // vertices
  // plus edges plus faces in the original mesh.  Basically we just need a
  // one-to-one
  // mapping between original mesh elements and subdivided mesh vertices.
  // [See subroutine for actual "TODO"s]
  assignSubdivisionIndices();

  // TODO Step III: Build a list of quads in the new (subdivided) mesh, as
  // tuples of
  // the element indices defined above.  In other words, each new quad should be
  // of
  // the form (i,j,k,l), where i,j,k and l are four of the indices stored on our
  // original mesh elements.  Note that it is essential to get the orientation
  // right
  // here: (i,j,k,l) is not the same as (l,k,j,i).  Indices of new faces should
  // circulate in the same direction as old faces (think about the right-hand
  // rule).
  // [See subroutines for actual "TODO"s]
  vector<vector<Index> > subDFaces;
  vector<Vector3D> subDVertices;
  buildSubdivisionFaceList(subDFaces);
  buildSubdivisionVertexList(subDVertices);

  // TODO Step IV: Pass the list of vertices and quads to a routine that clears
  // the
  // internal data for this halfedge mesh, and builds new halfedge data from
  // scratch,
  // using the two lists.
  rebuild(subDFaces, subDVertices);
}

/**
 * Compute new vertex positions for a mesh that splits each polygon
 * into quads (by inserting a vertex at the face midpoint and each
 * of the edge midpoints).  The new vertex positions will be stored
 * in the members Vertex::newPosition, Edge::newPosition, and
 * Face::newPosition.  The values of the positions are based on
 * simple linear interpolation, e.g., the edge midpoints and face
 * centroids.
 */
void HalfedgeMesh::computeLinearSubdivisionPositions() {
  // TODO For each vertex, assign Vertex::newPosition to
  // its original position, Vertex::position.

  // TODO For each edge, assign the midpoint of the two original
  // positions to Edge::newPosition.

  // TODO For each face, assign the centroid (i.e., arithmetic mean)
  // of the original vertex positions to Face::newPosition.  Note
  // that in general, NOT all faces will be triangles!
  showError("computeLinearSubdivisionPositions() not implemented.");
}

/**
 * Compute new vertex positions for a mesh that splits each polygon
 * into quads (by inserting a vertex at the face midpoint and each
 * of the edge midpoints).  The new vertex positions will be stored
 * in the members Vertex::newPosition, Edge::newPosition, and
 * Face::newPosition.  The values of the positions are based on
 * the Catmull-Clark rules for subdivision.
 */
void HalfedgeMesh::computeCatmullClarkPositions() {
  // TODO The implementation for this routine should be
  // a lot like HalfedgeMesh::computeLinearSubdivisionPositions(),
  // except that the calculation of the positions themsevles is
  // slightly more involved, using the Catmull-Clark subdivision
  // rules. (These rules are outlined in the Developer Manual.)

  // TODO face

  // TODO edges

  // TODO vertices
  showError("computeCatmullClarkPositions() not implemented.");
}

/**
 * Assign a unique integer index to each vertex, edge, and face in
 * the mesh, starting at 0 and incrementing by 1 for each element.
 * These indices will be used as the vertex indices for a mesh
 * subdivided using Catmull-Clark (or linear) subdivision.
 */
void HalfedgeMesh::assignSubdivisionIndices() {
  // TODO Start a counter at zero; if you like, you can use the
  // "Index" type (defined in halfedgeMesh.h)

  // TODO Iterate over vertices, assigning values to Vertex::index

  // TODO Iterate over edges, assigning values to Edge::index

  // TODO Iterate over faces, assigning values to Face::index
  showError("assignSubdivisionIndices() not implemented.");
}

/**
 * Build a flat list containing all the vertex positions for a
 * Catmull-Clark (or linear) subdivison of this mesh.  The order of
 * vertex positions in this list must be identical to the order
 * of indices assigned to Vertex::newPosition, Edge::newPosition,
 * and Face::newPosition.
 */
void HalfedgeMesh::buildSubdivisionVertexList(vector<Vector3D>& subDVertices) {
  // TODO Resize the vertex list so that it can hold all the vertices.

  // TODO Iterate over vertices, assigning Vertex::newPosition to the
  // appropriate location in the new vertex list.

  // TODO Iterate over edges, assigning Edge::newPosition to the appropriate
  // location in the new vertex list.

  // TODO Iterate over faces, assigning Face::newPosition to the appropriate
  // location in the new vertex list.
  showError("buildSubdivisionVertexList() not implemented.");
}

/**
 * Build a flat list containing all the quads in a Catmull-Clark
 * (or linear) subdivision of this mesh.  Each quad is specified
 * by a vector of four indices (i,j,k,l), which come from the
 * members Vertex::index, Edge::index, and Face::index.  Note that
 * the ordering of these indices is important because it determines
 * the orientation of the new quads; it is also important to avoid
 * "bowties."  For instance, (l,k,j,i) has the opposite orientation
 * of (i,j,k,l), and if (i,j,k,l) is a proper quad, then (i,k,j,l)
 * will look like a bowtie.
 */
void HalfedgeMesh::buildSubdivisionFaceList(vector<vector<Index> >& subDFaces) {
  // TODO This routine is perhaps the most tricky step in the construction of
  // a subdivision mesh (second, perhaps, to computing the actual Catmull-Clark
  // vertex positions).  Basically what you want to do is iterate over faces,
  // then for each for each face, append N quads to the list (where N is the
  // degree of the face).  For this routine, it may be more convenient to simply
  // append quads to the end of the list (rather than allocating it ahead of
  // time), though YMMV.  You can of course iterate around a face by starting
  // with its first halfedge and following the "next" pointer until you get
  // back to the beginning.  The tricky part is making sure you grab the right
  // indices in the right order---remember that there are indices on vertices,
  // edges, AND faces of the original mesh.  All of these should get used.  Also
  // remember that you must have FOUR indices per face, since you are making a
  // QUAD mesh!

  // TODO iterate over faces
  // TODO loop around face
  // TODO build lists of four indices for each sub-quad
  // TODO append each list of four indices to face list
  showError("buildSubdivisionFaceList() not implemented.");
}

FaceIter HalfedgeMesh::bevelVertex(VertexIter v) {
  // TODO This method should replace the vertex v with a face, corresponding to
  // a bevel operation. It should return the new face.  NOTE: This method is
  // responsible for updating the *connectivity* of the mesh only---it does not
  // need to update the vertex positions.  These positions will be updated in
  // HalfedgeMesh::bevelVertexComputeNewPositions (which you also have to
  // implement!)

  showError("bevelVertex() not implemented.");
  return facesBegin();
}

FaceIter HalfedgeMesh::bevelEdge(EdgeIter e) {
  // TODO This method should replace the edge e with a face, corresponding to a
  // bevel operation. It should return the new face.  NOTE: This method is
  // responsible for updating the *connectivity* of the mesh only---it does not
  // need to update the vertex positions.  These positions will be updated in
  // HalfedgeMesh::bevelEdgeComputeNewPositions (which you also have to
  // implement!)

  showError("bevelEdge() not implemented.");
  return facesBegin();
}

FaceIter HalfedgeMesh::bevelFace(FaceIter f) {
  // TODO This method should replace the face f with an additional, inset face
  // (and ring of faces around it), corresponding to a bevel operation. It
  // should return the new face.  NOTE: This method is responsible for updating
  // the *connectivity* of the mesh only---it does not need to update the vertex
  // positions.  These positions will be updated in
  // HalfedgeMesh::bevelFaceComputeNewPositions (which you also have to
  // implement!)

 
	//cout << "test" << endl;
	if (f->isBoundary()) { return f; }
    
	size_t sides = f->degree();

	vector<HalfedgeIter> new_h;
	vector<VertexIter> new_v;
	vector<EdgeIter> new_e;
	vector<FaceIter> new_face;

	HalfedgeIter old_h = f->halfedge();

	//initialize pointers to new elems
	for (size_t i = 0; i < sides; i++) {
		//add #sides faces
		new_face.push_back(newFace());
		//add #sides vertices
		new_v.push_back(newVertex());
		//add 2*#sides edges
		new_e.push_back(newEdge());
		new_e.push_back(newEdge());
		//add 4*#sides halfedges
		new_h.push_back(newHalfedge());
		new_h.push_back(newHalfedge());
		new_h.push_back(newHalfedge());
		new_h.push_back(newHalfedge());
	}
	
	//temp_h can save the value of old_h, before we change the next() of old_h
	HalfedgeIter temp_h;
	
	//set the relationship between elems
	for (size_t i = 0; i < sides; i++) {
		//old halfedge should change face and next
		old_h->face() = new_face[i];

		//initialize halfedge for newface
		new_face[i]->halfedge() = old_h;

		//initialize halfedge for newedge
		new_e[i * 2]->halfedge() = new_h[4 * i];
		new_e[i * 2 + 1]->halfedge() = new_h[i * 4 + 1];

		//intialize halfedge for newvertex
		new_v[i]->halfedge() = new_h[i * 4 + 1];
		new_v[i]->position = old_h->next()->vertex()->position;

		//intialize the value for newhalfedges
		new_h[i * 4]->setNeighbors(new_h[i * 4 + 1], new_h[((i + 1) % sides) * 4 + 2], old_h->next()->vertex(), new_e[i * 2], new_face[i]);
		new_h[i * 4 + 1]->setNeighbors(new_h[i * 4 + 2], new_h[i * 4 + 3], new_v[i], new_e[i * 2 + 1], new_face[i]);
		new_h[i * 4 + 2]->setNeighbors(old_h, new_h[((i - 1+sides) % sides) * 4], new_v[(i - 1+sides) % sides], new_e[((i - 1+sides) % sides) * 2], new_face[i]);
		new_h[i * 4 + 3]->setNeighbors(new_h[((i + 1) % sides) * 4 + 3], new_h[i * 4 + 1], new_v[(i - 1+sides) % sides], new_e[i * 2 + 1], f);

		//save the value before go to next
		temp_h = old_h;
		old_h = old_h->next();
		temp_h->next() = new_h[i * 4];
	}

	//assign halfedge to original face
	f->halfedge() = new_h[3];

	return f;
}


void HalfedgeMesh::bevelFaceComputeNewPositions(
    vector<Vector3D>& originalVertexPositions,
    vector<HalfedgeIter>& newHalfedges, double normalShift,
    double tangentialInset) {
  // TODO Compute new vertex positions for the vertices of the beveled face.
  //
  // These vertices can be accessed via newHalfedges[i]->vertex()->position for
  // i = 1, ..., newHalfedges.size()-1.
  //
  // The basic strategy here is to loop over the list of outgoing halfedges,
  // and use the preceding and next vertex position from the original mesh
  // (in the originalVertexPositions array) to compute an offset vertex
  // position.
  //
  // Note that there is a 1-to-1 correspondence between halfedges in
  // newHalfedges and vertex positions
  // in orig.  So, you can write loops of the form
  //
  // for( int i = 0; i < newHalfedges.size(); hs++ )
  // {
  //    Vector3D pi = originalVertexPositions[i]; // get the original vertex
  //    position correponding to vertex i
  // }
  //

}

void HalfedgeMesh::bevelVertexComputeNewPositions(
    Vector3D originalVertexPosition, vector<HalfedgeIter>& newHalfedges,
    double tangentialInset) {
  // TODO Compute new vertex positions for the vertices of the beveled vertex.
  //
  // These vertices can be accessed via newHalfedges[i]->vertex()->position for
  // i = 1, ..., hs.size()-1.
  //
  // The basic strategy here is to loop over the list of outgoing halfedges,
  // and use the preceding and next vertex position from the original mesh
  // (in the orig array) to compute an offset vertex position.

}

void HalfedgeMesh::bevelEdgeComputeNewPositions(
    vector<Vector3D>& originalVertexPositions,
    vector<HalfedgeIter>& newHalfedges, double tangentialInset) {
  // TODO Compute new vertex positions for the vertices of the beveled edge.
  //
  // These vertices can be accessed via newHalfedges[i]->vertex()->position for
  // i = 1, ..., newHalfedges.size()-1.
  //
  // The basic strategy here is to loop over the list of outgoing halfedges,
  // and use the preceding and next vertex position from the original mesh
  // (in the orig array) to compute an offset vertex position.
  //
  // Note that there is a 1-to-1 correspondence between halfedges in
  // newHalfedges and vertex positions
  // in orig.  So, you can write loops of the form
  //
  // for( int i = 0; i < newHalfedges.size(); i++ )
  // {
  //    Vector3D pi = originalVertexPositions[i]; // get the original vertex
  //    position correponding to vertex i
  // }
  //

}

void HalfedgeMesh::splitPolygons(vector<FaceIter>& fcs) {
  for (auto f : fcs) splitPolygon(f);
}

void HalfedgeMesh::splitPolygon(FaceIter f) {
  // TODO: (meshedit) 
  // Triangulate a polygonal face
  
	if (f->degree() == 3) { return; }
	size_t num_e = f->degree() - 3;
	size_t num_h = num_e * 2;

	vector<EdgeIter> new_e;
	vector<FaceIter> new_f;
	vector<HalfedgeIter> new_h;
	//alloct new edge, face and halfedge
	for (size_t i = 0; i < num_e; i++) {
		new_e.push_back(newEdge());
		new_f.push_back(newFace());
		new_h.push_back(newHalfedge());
		new_h.push_back(newHalfedge());
	}

	VertexIter v = f->halfedge()->vertex();
	HalfedgeIter h0 = f->halfedge();
	HalfedgeIter h1 = h0->next();
	v->halfedge() = h0;

	//deal with the first
	HalfedgeIter temp1 = h1->next();
	new_e[0]->halfedge() = new_h[0];
	h1->next() = new_h[0];
	new_h[0]->setNeighbors(h0, new_h[1], temp1->vertex(), new_e[0], f);

	//deal with the middle
	for (size_t j = 0; j < num_e - 1; j++) {
		HalfedgeIter temp2 = temp1;
		temp1 = temp1->next();
		new_h[2 * j + 1]->setNeighbors(temp2, new_h[2 * j], v, new_e[j], new_f[j]);
		new_h[2 * j + 2]->setNeighbors(new_h[2 * j + 1], new_h[2 * j + 3], temp1->vertex(), new_e[j + 1], new_f[j]);
		temp2->next() = new_h[2 * j + 2];
		new_f[j]->halfedge() = temp2;
		new_e[j + 1]->halfedge() = new_h[2 * j + 2];
	}

	//deal with the last
	//the last halfedge is new_h[num_h-1]
	temp1->face() = new_f[num_e - 1];
	temp1->next()->face() = new_f[num_e - 1];
	temp1->next()->next() = new_h[num_h - 1];
	new_h[num_h - 1]->setNeighbors(temp1, new_h[num_h - 2], v, new_e[num_e - 1], new_f[num_e - 1]);
	new_f[num_e - 1]->halfedge() = temp1;

}


EdgeRecord::EdgeRecord(EdgeIter& _edge) : edge(_edge) {
  // TODO: (meshEdit)
  // Compute the combined quadric from the edge endpoints.
	//EdgeRecord R;
	_edge->record.edge = _edge;
  // Compute the combined quadric matrix from the edge endpoints.
	Matrix4x4 quad = _edge->halfedge()->vertex()->quadric + _edge->halfedge()->twin()->vertex()->quadric;
  // -> Build the 3x3 linear system whose solution minimizes the quadric error
  //    associated with these two endpoints.
  // -> Use this system to solve for the optimal position, and store it in
  //    EdgeRecord::optimalPoint.
  // -> Also store the cost associated with collapsing this edg in
  //    EdgeRecord::Cost.
  Matrix3x3 A;
	A[0][0] = quad[0][0];
	A[0][1] = quad[0][1];
	A[0][2] = quad[0][2];
	A[1][0] = quad[1][2];
	A[1][1] = quad[1][1];
	A[1][2] = quad[1][2];
	A[2][0] = quad[2][0];
	A[2][1] = quad[2][1];
	A[2][2] = quad[2][2];
	Vector3D b;
	b[0] = -quad[0][3];
	b[1] = -quad[1][3];
	b[2] = -quad[2][3];
  // -> Use this system to solve for the optimal position, and store it in
  //    EdgeRecord::optimalPoint.
	Vector3D x = A.inv()*b;
	_edge->record.optimalPoint = x;
  // -> Also store the cost associated with collapsing this edg in
  //    EdgeRecord::Cost.
	Vector3D dist = _edge->centroid() + x;
	double cost = dist.norm();
	_edge->record.score = cost;

}

void MeshResampler::upsample(HalfedgeMesh& mesh)
// This routine should increase the number of triangles in the mesh using Loop
// subdivision.
{
  // TODO: (meshEdit)
  // Compute new positions for all the vertices in the input mesh, using
  // the Loop subdivision rule, and store them in Vertex::newPosition.
  // -> At this point, we also want to mark each vertex as being a vertex of the
  //    original mesh.
	for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
		v->isNew = false;
		double n = (double)(v->degree());
		double u;

		if (n == 3) u = 3.0 / 16.0;
		else u = 3.0 / (8.0 * n);

		v->newPosition = (1.0 - u*n)*v->position + u*(n*v->neighborhoodCentroid());
	}

  // -> Next, compute the updated vertex positions associated with edges, and
  //    store it in Edge::newPosition.
	for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
		HalfedgeIter temp1 = e->halfedge();
		HalfedgeIter temp2 = temp1->twin();
		e->newPosition = 3.0 / 8.0*2.0*e->centroid() + 1.0 / 8.0*(temp1->next()->next()->vertex()->position + temp2->next()->next()->vertex()->position);

		e->isNew = false;
	}

  // -> Next, we're going to split every edge in the mesh, in any order.  For
  //    future reference, we're also going to store some information about which
  //    subdivided edges come from splitting an edge in the original mesh, and
  //    which edges are new, by setting the flat Edge::isNew. Note that in this
  //    loop, we only want to iterate over edges of the original mesh.
  //    Otherwise, we'll end up splitting edges that we just split (and the
  //    loop will never end!)
  int n = mesh.nEdges();
	EdgeIter e = mesh.edgesBegin();
	vector<EdgeIter> E;
	vector<VertexIter> V;
	for (int i = 0; i < n; i++) {

		// get the next edge NOW!
		EdgeIter nextEdge = e;
		nextEdge++;

		VertexIter v0;
		if (e->isNew == false) {
			E.push_back(e);
			v0 = mesh.splitEdge(e);
			V.push_back(v0);
			v0->isNew = true;
			v0->halfedge()->twin()->next()->edge()->isNew = true;
			v0->halfedge()->next()->next()->edge()->isNew = true;
			//v0->halfedge()->twin()->next()->twin()->next()->edge()->isNew = true;
		}
		

		e = nextEdge;
	}
  
  // -> Now flip any new edge that connects an old and new vertex.
  for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
	  if ((e->isNew) && (e->halfedge()->vertex()->isNew ^ e->halfedge()->twin()->vertex()->isNew)) {
			mesh.flipEdge(e);
		}
  }

  // -> Finally, copy the new vertex positions into final Vertex::position.
  for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
		if (!v->isNew) v->position = v->newPosition;
		//else v->position = v->halfedge()->edge()->newPosition;
	}

	for (int j = 0; j < V.size(); j++) {
		V[j]->position = E[j]->newPosition;
	}

  // Each vertex and edge of the original surface can be associated with a
  // vertex in the new (subdivided) surface.
  // Therefore, our strategy for computing the subdivided vertex locations is to
  // *first* compute the new positions
  // using the connectity of the original (coarse) mesh; navigating this mesh
  // will be much easier than navigating
  // the new subdivided (fine) mesh, which has more elements to traverse.  We
  // will then assign vertex positions in
  // the new mesh based on the values we computed for the original mesh.

  // Compute updated positions for all the vertices in the original mesh, using
  // the Loop subdivision rule.

  // Next, compute the updated vertex positions associated with edges.

  // Next, we're going to split every edge in the mesh, in any order.  For
  // future
  // reference, we're also going to store some information about which
  // subdivided
  // edges come from splitting an edge in the original mesh, and which edges are
  // new.
  // In this loop, we only want to iterate over edges of the original
  // mesh---otherwise,
  // we'll end up splitting edges that we just split (and the loop will never
  // end!)

  // Finally, flip any new edge that connects an old and new vertex.

  // Copy the updated vertex positions to the subdivided mesh.
  showError("upsample() not implemented.");
}

void MeshResampler::downsample(HalfedgeMesh& mesh) {
  // TODO: (meshEdit)
  // Compute initial quadrics for each face by simply writing the plane equation
  // for the face in homogeneous coordinates. These quadrics should be stored
  // in Face::quadric
	for (FaceIter f = mesh.facesBegin(); f != mesh.facesEnd(); f++) {
		Vector3D N = f->normal();
		Vector3D p = f->centroid();
		double dist = -dot(N, p);
		Vector4D v = (N[0], N[1], N[2], dist);
		f->quadric = outer(v, v);
	}

  // -> Compute an initial quadric for each vertex as the sum of the quadrics
  //    associated with the incident faces, storing it in Vertex::quadric
  for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
		HalfedgeIter h = v->halfedge();
		Matrix4x4 quad = h->face()->quadric;
		while (h->next() != v->halfedge()) {
			h = h->next();
			quad += h->face()->quadric;
		}
	}

  // -> Build a priority queue of edges according to their quadric error cost,
  //    i.e., by building an EdgeRecord for each edge and sticking it in the
  //    queue.
  MutablePriorityQueue<EdgeRecord> queue;
	for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
		e->record = EdgeRecord(e);
		queue.insert(e->record);
	}

  // -> Until we reach the target edge budget, collapse the best edge. Remember
  //    to remove from the queue any edge that touches the collapsing edge
  //    BEFORE it gets collapsed, and add back into the queue any edge touching
  //    the collapsed vertex AFTER it's been collapsed. Also remember to assign
  //    a quadric to the collapsed vertex, and to pop the collapsed edge off the
  //    top of the queue.
  Size budget = 3 * mesh.nEdges() / 4;
	//cout << "current edge number" << mesh.nEdges() << endl;
	//cout << "should have: " << mesh.nEdges() - budget << endl;
	int i = 0;
	while (mesh.nEdges() > budget) {
		//cout << "round " << i << endl;
		i++;

		EdgeRecord best = queue.top();
		EdgeIter e = best.edge;
		queue.pop();
		
		VertexIter v0 = e->halfedge()->vertex();
		//VertexIter v1 = e->halfedge()->twin()->vertex();
		cout << "1" << endl;
		//remove all related edges
		HalfedgeIter temp1 = e->halfedge()->twin()->next();
		while (temp1 != e->halfedge()) {
			queue.remove(temp1->edge()->record);
			temp1 = temp1->twin()->next();
			
		}
		//cout << "2" << endl;
		HalfedgeIter temp2 = e->halfedge()->next();
		while (temp2 != e->halfedge()->twin()) {
			queue.remove(temp2->edge()->record);
			temp2 = temp2->twin()->next();
		}
		//cout << "3" << endl;
		//cout << "e: " << &(e->halfedge()) << endl;
		VertexIter v = mesh.collapseEdge(e);		
		//cout << "v degree: " << v->degree() << endl;
		//update quadric of related vertices, faces and edges
		HalfedgeIter hv = v->halfedge();
		Matrix4x4 mv;
		mv.zero();
		while (hv->twin()->next() != v->halfedge()) {
			mv += hv->face()->quadric;
			hv->edge()->record = EdgeRecord(hv->edge());
			queue.insert(hv->edge()->record);
			hv = hv->twin()->next();
		}
		v->quadric = mv;
		//cout << "--one round--" << endl;
	}
	//cout << "simplification end" << endl;
  //showError("downsample() not implemented.");
}

void MeshResampler::resample(HalfedgeMesh& mesh) {
  // TODO: (meshEdit)
  // Compute the mean edge length.
  // Repeat the four main steps for 5 or 6 iterations
  // -> Split edges much longer than the target length (being careful about
  //    how the loop is written!)
  // -> Collapse edges much shorter than the target length.  Here we need to
  //    be EXTRA careful about advancing the loop, because many edges may have
  //    been destroyed by a collapse (which ones?)
  // -> Now flip each edge if it improves vertex degree
  // -> Finally, apply some tangential smoothing to the vertex positions
  showError("resample() not implemented.");
}

}  // namespace CMU462
