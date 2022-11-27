/* 
 * Helper geometry classes.
 *
 * Copyright (c) Sergey B. Berezin (s_berezin@cs.msu.su), 2002.
 *
 * You may freely distribute or modify this source code retaining the 
 * above copyright notice.
 *
 * This software is provided "as-is" and without guarantee or warrantee 
 * of any kind, expressed or implied :)
 */

using System;

	public struct Vector2d {
		public double x,y;

		public Vector2d(double xx,double yy) {
			x = xx;
			y = yy;
		}

		public double Length {
			get {
				return Math.Sqrt(x*x + y*y);
			}
		}	

		public void Normalize() {
			double length = Length;
			if(length > 1e-10) {
				x /= length;
				y /= length;
			} else 
				x = y = 0;
		}

		public static Vector2d operator + (Vector2d a,Vector2d b) {
			return new Vector2d(a.x + b.x,a.y + b.y);
		}

		public static Vector2d operator - (Vector2d a,Vector2d b) {
			return new Vector2d(a.x - b.x,a.y - b.y);
		}

		public static Vector2d operator * (Vector2d a,double b) {
			return new Vector2d(a.x*b,a.y*b);
		}
	}

	public struct Vector3d {
		public double x,y,z;
	
		public Vector3d(double xi,double yi,double zi) {
			x = xi;
			y = yi;
			z = zi;
		}

		public double Length {
			get {
				return Math.Sqrt(x*x + y*y + z*z);
			}
		}

		public void MulAdd(Vector3d v1,Vector3d v2) {
			x += v1.y*v2.z-v2.y*v1.z;
			y += v1.z*v2.x-v2.z*v1.x;
			z += v1.x*v2.y-v2.x*v1.y;
		}

		public void MulAddN(Vector3d v1,Vector3d v2) {
			Vector3d v = v1*v2;
			v.Normalize();
			x += v.x;
			y += v.y;
			z += v.z;
		}

		public void Normalize() {
			double length = Math.Sqrt(x*x + y*y + z*z);
			if(length > 1e-8) {
				x /= length;
				y /= length;
				z /= length;
			}
		}

		static public Vector3d operator + (Vector3d v1,Vector3d v2) {
			return new Vector3d(v1.x + v2.x,v1.y + v2.y,v1.z + v2.z);
		}

		static public Vector3d operator - (Vector3d v1,Vector3d v2) {
			return new Vector3d(v1.x - v2.x,v1.y - v2.y,v1.z - v2.z);
		}

		static public Vector3d operator / (Vector3d v1,double d) {
			return new Vector3d(v1.x/d,v1.y/d,v1.z/d);
		}

		static public Vector3d operator * (Vector3d v1,Vector3d v2) {
			return new Vector3d(v1.y*v2.z-v2.y*v1.z,
				v1.z*v2.x-v2.z*v1.x,
				v1.x*v2.y-v2.x*v1.y);
		}

		static public Vector3d operator * (Vector3d v,double a) {
			return new Vector3d(v.x*a, v.y*a, v.z*a);
		}

		static public Vector3d operator * (double a,Vector3d v) {
			return new Vector3d(v.x*a, v.y*a, v.z*a);
		}

		static public double operator ^ (Vector3d v1,Vector3d v2) {
			return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
		}
	
	}
