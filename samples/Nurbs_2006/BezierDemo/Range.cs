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

	public struct RangeD {
		public double min,max;

		///
		/// <summary>Constructs range from pair of values</summary>
		/// 
		public RangeD(double x,double y) {
			if(x < y) {
				min = x;
				max = y;
			} else {
				min = y;
				max = x;
			}
		}

		public double Length {
			get {
				return (max - min);
			}
		}

		public double Center {
			get {
				return (min + max)/2;
			}
		}

		public void Increase(double delta) {
			min -= delta/2;
			max += delta/2;
		}

		public double Scale(double x,RangeD dst) {
			return dst.min + dst.Length*(x - min)/Length;
		}

		public void Unite(RangeD another) {
			if(min > another.min)
				min = another.min;
			if(max < another.max)
				max = another.max;
		}
	}
