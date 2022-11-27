using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

	public class BezierDisplay : PointEditor
	{
		/// <summary> Required designer variable. </summary>
		private System.ComponentModel.Container components = null;

		/// <summary> Bezier curve </summary>
		protected BezierCurve bezier = null;
		protected BSplineBasis bspline = null;

		public BezierDisplay()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			// TODO: Add any initialization after the InitComponent call
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if( components != null )
					components.Dispose();
			}
			base.Dispose( disposing );
		}

		#region Component Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			components = new System.ComponentModel.Container();
		}
		#endregion

		protected override void OnPaint(PaintEventArgs pe)
		{
			// Paint bezier curve
			if(bezier != null) {
				Pen pen = new Pen(Color.Red,2);
				Vector2d prev = bezier.GetPoint(0);
				for(double t = 0.01;t<1.01;t+=0.01) {
					Vector2d v = bezier.GetPoint(t);
					pe.Graphics.DrawLine(pen,
						viewport.GetScreenX(prev.x),
						viewport.GetScreenY(prev.y),
						viewport.GetScreenX(v.x),
						viewport.GetScreenY(v.y));
					prev = v;
				}
				pen.Dispose();
			}
			if(bspline != null) {
				Pen pen1 = new Pen(Color.Green,2);
				Vector2d prev = new Vector2d(0,0);
				for(int i = 0;i<bspline.PointCount;i++)
					prev += bezier.Points[i]*bspline.b[0,i];
				for(int k = 1;k<bspline.Tesselation;k++) {
					Vector2d v = new Vector2d(0,0);
					for(int i = 0;i<bspline.PointCount;i++)
						v += bezier.Points[i]*bspline.b[k,i];
					pe.Graphics.DrawLine(pen1,
							viewport.GetScreenX(prev.x),
							viewport.GetScreenY(prev.y),
							viewport.GetScreenX(v.x),
							viewport.GetScreenY(v.y));
					prev = v;
				}
				pen1.Dispose();
			}
			base.OnPaint(pe);
		}

		protected override void OnPointAdded(int newIndex) { 
			if(points.Count > 1) 
				bezier = new BezierCurve(points);
			if(points.Count > 3)
				bspline = new BSplineBasis(BSplineBasisType.Open,points.Count,3,0,1);
			base.OnPointAdded(newIndex);
		}

		protected override void OnPointChanged(int index) {
			if(bezier != null) 
				bezier.Points[index] = (Vector2d)points[index];
			Refresh();
			base.OnPointChanged(index);
		}
	}

