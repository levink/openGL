using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace BezierDemo
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private PointEditor pointEditor;
		private BezierDisplay bezierDisplay1;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			SetStyle(ControlStyles.Opaque,true);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.bezierDisplay1 = new BezierDisplay();
			this.SuspendLayout();
			// 
			// bezierDisplay1
			// 
			this.bezierDisplay1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.bezierDisplay1.Location = new System.Drawing.Point(0, 0);
			this.bezierDisplay1.Name = "bezierDisplay1";
			this.bezierDisplay1.Size = new System.Drawing.Size(632, 321);
			this.bezierDisplay1.TabIndex = 0;
			this.bezierDisplay1.Text = "bezierDisplay1";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(632, 321);
			this.Controls.Add(this.bezierDisplay1);
			this.Name = "Form1";
			this.Text = "Кривые Безье";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}
	}
}
