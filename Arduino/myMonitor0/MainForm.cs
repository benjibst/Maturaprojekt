/*
 * Created by SharpDevelop.
 * User: Josef Pedevilla
 * Date: 20.10.2020
 * Time: 22:13
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;

namespace myMonitor0
{
	/// <summary>
	/// Description of MainForm.
	/// </summary>
	public partial class MainForm : Form
	{
		
		public MainForm()
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			buttonConnect.BackColor=Color.Coral;
			
		}
		
		void SerialPort1DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
		{
			Invoke(new Action(() =>
			{
				textBoxIN.AppendText(serialPort1.ReadExisting());
			}));
		}
		
		void ButtonConnectClick(object sender, EventArgs e)
		{
			if(serialPort1.IsOpen==false)
				{
					try
					{
						serialPort1.PortName = textBoxCOM.Text;
						serialPort1.Open();
						buttonConnect.Text = "disconnect";
						buttonConnect.BackColor=Color.LightGreen;
						
					}
					catch
					{
						MessageBox.Show("Bitte den richtigen COM-PORT auswählen!");
					}
						
				}				
				else
				{
					serialPort1.Close();
					buttonConnect.Text = "Connect";
					buttonConnect.BackColor=Color.Coral;
				}			
		}

		protected override void OnFormClosing(FormClosingEventArgs e)
		{ serialPort1.Close(); }
		
		void ButtonResetClick(object sender, EventArgs e)
		{
			textBoxIN.Text="";	
		}

        private void button1_Click(object sender, EventArgs e)
        {
			serialPort1.DiscardInBuffer();
        }
    }
}
