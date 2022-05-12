/*
 * Created by SharpDevelop.
 * User: Josef Pedevilla
 * Date: 20.10.2020
 * Time: 22:13
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace myMonitor0
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		private System.Windows.Forms.Button buttonConnect;
		private System.Windows.Forms.TextBox textBoxCOM;
		private System.Windows.Forms.Button buttonReset;
		private System.IO.Ports.SerialPort serialPort1;
		private System.Windows.Forms.TextBox textBoxIN;
		private System.Windows.Forms.CheckBox checkBoxOverwrite;
		
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.buttonConnect = new System.Windows.Forms.Button();
			this.textBoxCOM = new System.Windows.Forms.TextBox();
			this.textBoxIN = new System.Windows.Forms.TextBox();
			this.buttonReset = new System.Windows.Forms.Button();
			this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
			this.checkBoxOverwrite = new System.Windows.Forms.CheckBox();
			this.SuspendLayout();
			// 
			// buttonConnect
			// 
			this.buttonConnect.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.125F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.buttonConnect.Location = new System.Drawing.Point(43, 37);
			this.buttonConnect.Name = "buttonConnect";
			this.buttonConnect.Size = new System.Drawing.Size(197, 57);
			this.buttonConnect.TabIndex = 0;
			this.buttonConnect.Text = "connect";
			this.buttonConnect.UseVisualStyleBackColor = true;
			this.buttonConnect.Click += new System.EventHandler(this.ButtonConnectClick);
			// 
			// textBoxCOM
			// 
			this.textBoxCOM.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.125F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.textBoxCOM.Location = new System.Drawing.Point(43, 126);
			this.textBoxCOM.Name = "textBoxCOM";
			this.textBoxCOM.Size = new System.Drawing.Size(100, 38);
			this.textBoxCOM.TabIndex = 1;
			this.textBoxCOM.Text = "COM4";
			// 
			// textBoxIN
			// 
			this.textBoxIN.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.125F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.textBoxIN.Location = new System.Drawing.Point(301, 37);
			this.textBoxIN.Multiline = true;
			this.textBoxIN.Name = "textBoxIN";
			this.textBoxIN.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.textBoxIN.Size = new System.Drawing.Size(348, 547);
			this.textBoxIN.TabIndex = 2;
			// 
			// buttonReset
			// 
			this.buttonReset.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.125F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.buttonReset.Location = new System.Drawing.Point(43, 527);
			this.buttonReset.Name = "buttonReset";
			this.buttonReset.Size = new System.Drawing.Size(197, 57);
			this.buttonReset.TabIndex = 3;
			this.buttonReset.Text = "reset";
			this.buttonReset.UseVisualStyleBackColor = true;
			this.buttonReset.Click += new System.EventHandler(this.ButtonResetClick);
			// 
			// serialPort1
			// 
			this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.SerialPort1DataReceived);
			// 
			// checkBoxOverwrite
			// 
			this.checkBoxOverwrite.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.125F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.checkBoxOverwrite.Location = new System.Drawing.Point(43, 467);
			this.checkBoxOverwrite.Name = "checkBoxOverwrite";
			this.checkBoxOverwrite.Size = new System.Drawing.Size(184, 42);
			this.checkBoxOverwrite.TabIndex = 4;
			this.checkBoxOverwrite.Text = "overwrite";
			this.checkBoxOverwrite.UseVisualStyleBackColor = true;
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(716, 621);
			this.Controls.Add(this.checkBoxOverwrite);
			this.Controls.Add(this.buttonReset);
			this.Controls.Add(this.textBoxIN);
			this.Controls.Add(this.textBoxCOM);
			this.Controls.Add(this.buttonConnect);
			this.Name = "MainForm";
			this.Text = "myMonitor0";
			this.ResumeLayout(false);
			this.PerformLayout();

		}
	}
}
