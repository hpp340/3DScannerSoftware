/*!
*      \file Zipper.h
*      \brief Algorithm for Gauss Curvature
*	   \author David Gu
*      \date Document 02/13/2014
*
*/


#ifndef _ZIPPER_H_
#define _ZIPPER_H_

#include <vector>
#include "Mesh/iterators.h"
#include "ZipperMesh.h"

using namespace std;

namespace MeshLib
{

	template<typename M>
	class CZipper
	{
	public:
		/*!	CZipper constructor
		 *	\param pMesh the input mesh
		 */
		CZipper(M* pMesh);
		/*!	CZipper destructor
		 */
		~CZipper();

		/*!
		 *	maps the complement of the arc (z_0, z_1, z_2) to the upper half plane
		 */
		void _map_arc_completment_to_upper_half_plane();
		/*!
		 *	upper half plane to the unit disk
		 */
		std::complex<double> _map_upper_half_plane_to_unit_disk(std::complex<double> z);
		/*!
		 *	unit disk to upper half plane
		 */

		std::complex<double> _map_unit_disk_to_upper_half_plane(std::complex<double> z);
		/*!
		 *	normalize
		 */

		void _normalize();
		/*!
		 *	unzip one segment
		 */
		void _zip();

		/*!
		 *	boundary vertices
		 */
		typename M::CVertex * operator[](size_t k) { return m_pts[k]; };

		/*!
		 *	output the uv coordinates
		 */
		void output(const char * fileName);

	protected:
		/*!	The input surface mesh
		 */
		M* m_pMesh;
		/*	boundary
		 */
		typename M::CBoundary m_boundary;
		/*	array of boundary points
		 */
		std::vector<typename M::CVertex*> m_pts;
		int m_order;
	};


	/*!	CHarmonicMapper constructor
	*	Count the number of interior vertices, boundary vertices and the edge weight
	*
	*/
	template<typename M>
	CZipper<M>::CZipper(M* pMesh) : m_pMesh(pMesh), m_boundary(pMesh)
	{
		M::CLoop * pL = m_boundary.loops()[0];

		for (std::list<M::CHalfEdge*>::iterator hiter = pL->halfedges().begin(); hiter != pL->halfedges().end(); hiter++)
		{
			M::CHalfEdge * pH = *hiter;
			M::CVertex   * pV = m_pMesh->halfedgeTarget(pH);
			m_pts.push_back(pV);
		}

	};

	/*!
	 *	CZipper destructor
	 */
	template<typename M>
	CZipper<M>::~CZipper()
	{
	};

	/*FIRST STEP OF ZIPPER ALGORITHM*/

	std::complex<double> zipper_1(std::complex<double> z, std::complex<double> z_0, std::complex<double> z_1, std::complex<double> z_2)
	{
		//complex<double> test = { 2, 0 };
		//return test;
		if (z == z_2)
		{
			complex<double> www = { 9999999, 9999999 };
			return www;
		}
		else
		{
			std::complex<double> w = { 0, 0 };
			complex<double> multiplier = (z_1 - z_2) / (z_1 - z_0);
			w = ((z - z_0) / (z - z_2)) * multiplier;
			complex<double> ww = sqrt(w);
			if (ww.imag() < 0)
			{
				ww = { -ww.real(), -ww.imag() };
			}
			//complex<double> test = { -1, 0 };
			return ww;
		}
	}

	/*MAIN PART OF ZIPPER ALGORITHM*/
	std::complex<double> zipper_main(complex<double> vert, std::complex<double> z)
	{
		double bb = vert.imag() * vert.imag() / vert.real() + vert.real();
		complex<double> b = { bb, 0 };
		complex<double> real1 = { 1, 0 };
		complex<double> w_0 = z / (real1 - z / b);
		//if (norm((real1 - z / b)) < 1e-3)
		//{
		//	w_0 = { 9999999, 9999999 };
		//}
		//complex<double> c = vert / (real1 - vert / b);
		double c = (vert.real()*vert.real() + vert.imag()*vert.imag()) / vert.imag();
		//complex<double> w_1 = sqrt(w_0 * w_0 + c.imag() * c.imag());
		complex<double> w_1 = sqrt(w_0 * w_0 + c*c);
		if (abs(w_1.imag()) < 1e-3)
		{
			w_1 = { w_1.real(), 0 };
		}

		if (w_1.imag() < 0)
		{
			w_1 = { -w_1.real(), -w_1.imag() };
		}

		if (abs(w_1.real()) < 1e-3)
		{
			w_1 = { 0, w_1.imag() };
		}

		return w_1;
	}

	/*!
	*	maps the completment of the circular arc through z0,z1,z2 to upper half plane
	*/
	template<typename M>
	void CZipper<M>::_map_arc_completment_to_upper_half_plane()
	{
		// fill your code here
		// Implemented according to the Fig 2.5 and Fig 2.6 in Reading Material Riemann Mapping
		m_order = 3;
		typename M::CVertex * vertMesh0 = m_pts[0];
		typename M::CVertex * vertMesh1 = m_pts[1];
		typename M::CVertex * vertMesh2 = m_pts[2];

		complex<double> vert_0 = vertMesh0->z();
		complex<double> vert_1 = vertMesh1->z();
		complex<double> vert_2 = vertMesh2->z();
		//cout << zipper_1(vert_2, vert_0, vert_1, vert_2) << endl;
		//getchar();
		// First step: w = \sqrt{(z-z1)/(z-z0)} - ONLY ONCE
		for (typename M::MeshVertexIterator viter(m_pMesh); !viter.end(); viter++)
		{
			typename M::CVertex * vert = *viter;
			complex<double> vert_complex = vert->z();
			complex<double> vert_C_step1 = zipper_1(vert_complex, vert_0, vert_1, vert_2);
			vert->z() = vert_C_step1;
		}
	};

	/*!
	 *	unzip one segment
	 */
	template<typename M>
	void CZipper<M>::_zip()
	{
		//fill your code here
		m_order++;
		// we have mapped {m_pts[1] m_pts[2] m_pts[3]} to {0, 1, infinite point}
		// first map m_pts[0] (when m_order = 4) and then m_pts[n-1] m_pts[n-2] ... m_pts[4]
		// add the boundary points in a vector

		cout << endl;
		complex<double> a;
		int n = m_pts.size();
		if (m_order > n)
		{
			cout << "Zipper Ended!" << endl;
			exit(0);
		}

		typename M::CVertex * vertMesh_k = m_pts[n - (m_order - 3)];
		a = vertMesh_k->z();
		cout << "a is " << a << endl;

		cout << "z_2 " << m_pts[2]->z() << endl;
		for (typename M::MeshVertexIterator viter(m_pMesh); !viter.end(); viter++)
		{
			typename M::CVertex * vert = *viter;
			complex<double> vert_c = vert->z();
			complex<double> vert_after_zipper = zipper_main(a, vert_c);
			vert->z() = vert_after_zipper;
		}

		cout << "z_2_after " << m_pts[2]->z() << endl;

		cout << m_order << "/" << n << endl;
	};


	/*!
	 *	maps the upper half plane to the unit disk
	 */
	template<typename M>
	std::complex<double> CZipper<M>::_map_upper_half_plane_to_unit_disk(std::complex<double> z)
	{
		std::complex<double> zi(0, 1);

		std::complex<double> w = (z - zi) / (z + zi);
		return w;
	};

	/*!
	 *	maps the upper half plane to the unit disk
	 */
	template<typename M>
	std::complex<double> CZipper<M>::_map_unit_disk_to_upper_half_plane(std::complex<double> z)
	{
		std::complex<double> zi(0, 1);

		std::complex<double> w = zi*(1.0 + z) / (1.0 - z);
		return w;
	};

	/*!
	 *	maps the upper half plane to the unit disk
	 */
	template<typename M>
	void CZipper<M>::_normalize()
	{
		std::complex<double> w(0, 0);

		for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); viter++)
		{
			M::CVertex * pV = *viter;
			std::complex<double> zeta = _map_upper_half_plane_to_unit_disk(pV->z());
			w += zeta;
		}
		w /= m_pMesh->numVertices();

		w = _map_unit_disk_to_upper_half_plane(w);

		std::complex<double> r = std::complex<double>(0, 1) / w;

		double a = r.real();
		double b = -r.imag();

		w = a * w / (b * w + 1.0);
		std::cout << w << std::endl;

		for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); viter++)
		{
			M::CVertex * pV = *viter;
			std::complex<double> z = pV->z();
			z = a * z / (b * z + 1.0);
			pV->z() = z;
		}

	};


	/*!
	 *	output mesh
	 */
	template<typename M>
	void CZipper<M>::output(const char * file_name)
	{
		for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); viter++)
		{
			M::CVertex * pV = *viter;
			pV->z() = _map_upper_half_plane_to_unit_disk(pV->z());
		}

		m_pMesh->write_m(file_name);
	};
};
#endif

