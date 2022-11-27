using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

public delegate void PointAddedHandler(int index);
public delegate void PointChangedHandler(int index);

public class PointEditor : Control {

	/// <summary>Required designer variable.</summary>
	private System.ComponentModel.Container components = null;

	/// <summary>For translation from logical coordinates to window</summary>
	protected Viewport2d viewport = new Viewport2d();

	/// <summary>List of control points</summary>
	protected ArrayList points = new ArrayList();

	/// <summary>Index of selected point or -1 if no point is currently selected</summary>
	protected int selectedIndex = -1;

	/// <summary>Graphics objects for drawing</summary>
	protected Pen pointPen = new Pen(Color.Green,2);
	protected Pen borderPen = new Pen(Color.Black,1);
	protected SolidBrush textBrush = new SolidBrush(Color.Black);
	protected Font font = new Font("Arial",14);

	/// <summary>Point radius on screen (should be property)</summary>
	public int PointRadius = 3;

	/// <summary>This event is raised when new point is added</summary>
	public event PointAddedHandler PointAdded;

	/// <summary>This event is raised when point coordinates are changed</summary>
	public event PointChangedHandler PointChanged;

	public PointEditor() {
		// This call is required by the Windows.Forms Form Designer.
		InitializeComponent();
		viewport.Margin = 0;
		viewport.SetDomain(new RectangleD(-1,-1,1,1));
		SetStyle(ControlStyles.ResizeRedraw,true);
		SetStyle(ControlStyles.AllPaintingInWmPaint,true);
//		SetStyle(ControlStyles.Opaque,true);
		SetStyle(ControlStyles.UserPaint,true);
		SetStyle(ControlStyles.DoubleBuffer,true);

	}

	/// <summary> Clean up any resources being used. </summary>
	protected override void Dispose(bool disposing) {
		if(disposing) {
			if(components != null)
				components.Dispose();
			pointPen.Dispose();
			borderPen.Dispose();
			textBrush.Dispose();
			font.Dispose();
		}
		base.Dispose( disposing );
	}

	#region Component Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify 
	/// the contents of this method with the code editor.
	/// </summary>
	private void InitializeComponent() {
		// 
		// PointEditor
		// 
		this.BackColor = System.Drawing.SystemColors.Window;
		this.SizeChanged += new System.EventHandler(this.OnSizeChanged);
		this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
		this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnMouseMove);
		this.MouseLeave += new System.EventHandler(this.OnMouseLeave);
		this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);

	}
	#endregion

	protected override void OnPaint(PaintEventArgs pe) {
		int x0 = viewport.GetScreenX(-1);
		int y0 = viewport.GetScreenY(-1);
		pe.Graphics.DrawRectangle(borderPen,
			x0,y0,viewport.GetScreenX(1) - x0,viewport.GetScreenY(1) - y0);
		for(int i = 0;i<points.Count;i++) 
			DrawPoint(pe.Graphics,(Vector2d)points[i],i+1);
		base.OnPaint(pe);
	}

	private void DrawPoint(Graphics g,Vector2d v,int index) {
		int x = viewport.GetScreenX(v.x);
		int y = viewport.GetScreenY(v.y);
		g.DrawEllipse(pointPen,x-PointRadius,y-PointRadius,2*PointRadius,2*PointRadius);
		g.DrawString(index.ToString(),font,textBrush,new PointF(x+2,y+3));
	}

	private void OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs e) {
		if(e.Button == MouseButtons.Left) {
			int index = FindPoint(e.X,e.Y);
			if(index == -1) {
				double x = viewport.GetViewX(e.X);
				double y = viewport.GetViewY(e.Y);
				points.Add(new Vector2d(x,y));
				OnPointAdded(points.Count - 1);
				Invalidate();
			} else {
				selectedIndex = index;
			}
		}
	}

	protected virtual void OnPointAdded(int newIndex) {
		if(PointAdded != null)
			PointAdded(newIndex);
	}

	protected virtual void OnPointChanged(int index) {
		if(PointChanged != null)
			PointChanged(index);
	}

	private void OnSizeChanged(object sender, System.EventArgs e) {
		viewport.Window = new Rectangle(0,0,Width,Height);
	}

	protected int FindPoint(int x,int y) {
		for(int i = 0;i<points.Count;i++) {
			Vector2d v = (Vector2d)points[i];
			int x2 = viewport.GetScreenX(v.x);
			int y2 = viewport.GetScreenY(v.y);
			if(Math.Sqrt((x - x2)*(x - x2) + (y - y2)*(y - y2)) < 2*PointRadius)
				return i;
		}
		return -1;
	}

	private void OnMouseMove(object sender, System.Windows.Forms.MouseEventArgs e) {
		if(selectedIndex != -1) {
			points[selectedIndex] = new Vector2d(viewport.GetViewX(e.X),viewport.GetViewY(e.Y));
			OnPointChanged(selectedIndex);
		}
	}

	private void OnMouseUp(object sender, System.Windows.Forms.MouseEventArgs e) {
		if(e.Button == MouseButtons.Left) {
			if(selectedIndex != -1)
				selectedIndex = -1;
		}
	}

	private void OnMouseLeave(object sender, System.EventArgs e) {
		if(selectedIndex != -1)
			selectedIndex = -1;
	}
}