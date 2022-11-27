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
using System.Diagnostics;
using System.Drawing;

	/*
	 * Logical data structure representing a stack of 2D viewports
	 */

	public class Viewport2d {
		protected int margin;         // Min. distance from window border to output area
		protected Rectangle window;   // Screen coordinates of window
		protected Rectangle output;   // Screen coordinates of output area inside window
		protected RectangleD visible; // View coordinates of output area
		protected RectangleD[] domains;   // Stack of enclosed domains, where domain[0] is the largest
		protected double aspect;      // Aspect ratio

		public Viewport2d() {
			margin = 15;  
			output = new Rectangle(0,0,0,0);
			window = new Rectangle(0,0,0,0);
			visible = new RectangleD();
			domains = new RectangleD[1];
			domains[0] = new RectangleD(-1,-1,1,1);
			aspect = 1; 
		}

		public int GetScreenX(double x) {
			Debug.Assert(!visible.IsEmpty);
			return (int)(output.Left + output.Width*(x - visible.Xmin)/visible.Width);
		}

		public int GetScreenY(double y) {
			Debug.Assert(!visible.IsEmpty);
			return (int)(output.Top + output.Height*(y - visible.Ymin)/visible.Height);
		}

		public double GetScreenXd(double x) {
			Debug.Assert(!visible.IsEmpty);
			return output.Left + output.Width*(x - visible.Xmin)/visible.Width;
		}

		public double GetScreenYd(double y) {
			Debug.Assert(!visible.IsEmpty);
			return output.Top + output.Height*(y - visible.Ymin)/visible.Height;
		}

		public double GetViewX(int x) {
			Debug.Assert(!output.IsEmpty);
			double vx = visible.Xmin + visible.Width*(x - output.X)/output.Width;
			if(vx < EntireDomain.Xmin)
				vx = EntireDomain.Xmin;
			else if(vx > EntireDomain.Xmax)
				vx = EntireDomain.Xmax;
			return vx;
		}

		public double GetViewY(int y) {
			Debug.Assert(!output.IsEmpty);
			double vy = visible.Ymin + visible.Height*(y - output.Y)/output.Height;
			if(vy < EntireDomain.Ymin)
				vy = EntireDomain.Ymin;
			else if(vy > EntireDomain.Ymax)
				vy = EntireDomain.Ymax;
			return vy;
		}

		public double Xmin {
			get { return visible.Xmin; }
		}

		public double Ymin {
			get { return visible.Ymin; }
		}

		public double Xmax {
			get { return visible.Xmax; }
		}

		public double Ymax {
			get { return visible.Ymax; }
		}

		public RectangleD Visible {
			get { return visible; }
		}

		// Domain control
		public RectangleD CurrentDomain {
			get { return domains[domains.Length - 1]; }
		}

		public RectangleD EntireDomain {
			get { return domains[0]; }
		}
	
		public void SetDomain(RectangleD d) {
			domains = new RectangleD[1];
			domains[0] = d;
			UpdateRects();
		}

		// Zooming
		public void ZoomIn(RectangleD d) {
			RectangleD[] newDomains = new RectangleD[domains.Length + 1];
			for(int i = 0;i<domains.Length;i++)
				newDomains[i] = domains[i];
			newDomains[domains.Length] = d;
			domains = newDomains;
			UpdateRects();
		}

		public void ZoomIn(Rectangle r) {
			ZoomIn(new RectangleD(GetViewX(r.X),
				GetViewY(r.Y),
				GetViewX(r.X + r.Width),
				GetViewY(r.Y + r.Height)));
		}

		public void ZoomOutToOriginal() {
			RectangleD[] newDomains = new RectangleD[1];
			newDomains[0] = domains[0];
			domains = newDomains;
			UpdateRects();
		}

		// Window control
		public Rectangle Window {
			get {
				return window;
			}
			set {
				window = value;
				UpdateRects();
			}
		}

		public int Margin {
			get {
				return margin;
			}
			set {
				margin = value;
				UpdateRects();
			}
		}

		public double Aspect {
			get {
				return aspect; 
			}
			set {
				aspect = value;
				UpdateRects();
			}
		}

		public Rectangle Output {
			get {
				return output;
			}
		}

		public bool IsEmpty {
			get { return output.IsEmpty; }
		}

		// Updates visible and output according to margin, window, domains and aspect
		protected void UpdateRects() {
			output = window;
			output.Inflate(-margin,-margin);
			if(output.IsEmpty)
				return;
			int w = output.Width;      // Dimensions of output window
			int h = output.Height;
			visible = CurrentDomain;
			double wd = visible.Width; // Dimensions of visible part of domain
			double hd = visible.Height;
			if(wd*h*aspect > w*hd) {        // m_aspect*wd/hd > w/h
				double y = wd*aspect*h/w - hd;
				double ymin = EntireDomain.Ymin;
				if(visible.Ymin - ymin < y/2) {
					y -= visible.Ymin - ymin;
					visible.y.min = ymin;
				} else {
					visible.y.min -= y/2;
					y/=2;
				}
				double ymax = EntireDomain.Ymax;
				if(ymax - visible.Ymax < y) {
					y -= ymax - visible.Ymax;
					visible.y.max = ymax;
					hd = visible.Height;
					int delta = (int)(h - w*hd/(aspect*wd));
					output.Y += (delta >> 1);
					output.Height -= delta;
				} else 
					visible.y.max += y;
			} else if(wd*h*aspect < w*hd) { // aspect*wd/hd < w/h
				double x = w*hd/(aspect*h)  - wd;
				double xmin = EntireDomain.Xmin;
				if(visible.Xmin - xmin < x/2) {
					x -= visible.Xmin - xmin;
					visible.x.min = xmin;
				} else {
					visible.x.min -= x/2;
					x/=2;
				}
				double xmax = EntireDomain.Xmax;
				if(xmax - visible.Xmax < x) {
					x -= xmax - visible.Xmax;
					visible.x.max = xmax;
					wd = visible.Width;
					int delta = (int)(w - wd*aspect*h/hd);
					output.X += (delta >> 1);
					output.Width -= delta;
				} else
					visible.x.max += x;
			} 
		}
	}
