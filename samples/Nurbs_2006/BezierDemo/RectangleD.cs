/* 
 * Copyright (c) Sergey B. Berezin (s_berezin@cs.msu.su), 2002, 2003.
 *
 * You may freely distribute or modify this source code retaining the 
 * above copyright notice.
 *
 * This software is provided "as-is" and without guarantee or warrantee 
 * of any kind, expressed or implied :)
 */

using System;

/*
 *  Rectangle with double precision coordinates
 */

	public struct RectangleD {
		public RangeD x,y;

		public RectangleD(double x1,double y1,double x2,double y2) {
			x = new RangeD(x1,x2);
			y = new RangeD(y1,y2);
		}

		public double Xmin {
			get {
				return x.min; 
			}
		}

		public double Xmax {
			get {
				return x.max;
			}
		}

		public double Ymin {
			get {
				return y.min;
			}
		}

		public double Ymax {
			get {
				return y.max;
			}
		}

		public double Width {
			get { 
				return x.Length; 
			}
		}

		public double Height {
			get { 
				return y.Length; 
			}
		}

		public bool IsEmpty {
			get { 
				return (Xmin >= Xmax && Ymin >= Ymax); 
			}
		}

		private static RectangleD emptyRectangle = new RectangleD(0,0,0,0);
  
		public static RectangleD Empty {
			get {
				return emptyRectangle;
			}
		}
	}
