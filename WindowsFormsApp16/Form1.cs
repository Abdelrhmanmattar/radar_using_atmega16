using System;
using System.IO.Ports;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp16
{
    public partial class Form1 : Form
    {
        private List<Point> drawnPoints = new List<Point>();

        private Point radarCenter , radarHead;
        private int radius;
        private double angle;
        Bitmap bwm;
        Graphics graphics;
        Pen pen;
        double distance;
        PointF point;
        static SerialPort serialPort;
        

        bool stop=true;
        public Form1()
        {
            InitializeComponent();
            WindowState = FormWindowState.Maximized;

            serialPort = new SerialPort("COM10", 9600);
            serialPort.DataBits = 8;
            serialPort.StopBits = StopBits.One;
            serialPort.Parity = Parity.None;
            serialPort.Handshake = Handshake.None;
            serialPort.DataReceived += SerialPortDataReceived;
            bwm = new Bitmap(this.Width + 1, this.Height + 1);
            this.BackColor = Color.Black;

            try
            {
                serialPort.Open();
                MessageBox.Show("CONNECT OPEN");
                serialPort.Close();
            }
            catch (Exception E)
            {
                MessageBox.Show(E.Message);
            }
            try
            {
                serialPort.Open();
            }
            catch(Exception e)
            {
                MessageBox.Show(e.Message);
            }
            InitializeRadar();
        }
 
        private void SerialPortDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (stop) return;
            string data = serialPort.ReadTo("\n");
            string[] strings = data.Split(' ');
            int dis=-1, an=-1;
            double R=-1;
            string s;

            try
            {
                an = int.Parse(strings[1]);
                R = an * Math.PI / 180.0;
            }
            catch { }
            if (strings[0] == "-1") s = "no";
            else s = strings[0];

            BeginInvoke(new Action(() =>
            {
                this.label2.Text = "DISTANCE = "+s + " ANGLE=" + strings[1];
            }));



            if (R <= 0.0 || R >= 3.14)
            {
                drawnPoints.Clear();
                angle = R;
                bwm = new Bitmap(this.Width + 1, this.Height + 1);
                paint_every();
                this.Invalidate();
            }
            if (strings[0]!="-1")
            {
                dis = Convert.ToInt32(strings[0]);
                an = Convert.ToInt32(strings[1]);
                point = DRAW_POINT(dis, an);
                PaintReceivedPoint((int)point.X,(int) point.Y);
            }
            return;
        }

        private void PaintReceivedPoint(int x, int y)
        {
            try
            {
                drawnPoints.Add(new Point(x, y));
                graphics = Graphics.FromImage(bwm);

                pen = new Pen(Color.Yellow, 5);
                graphics.DrawEllipse(pen, x - 4, y - 4, 6, 6);
                graphics.Dispose();
                pen.Dispose();
                this.pictureBox1.Image = bwm;
            }
            catch { }
        }



        private void InitializeRadar()
        {
            radarCenter = new Point(ClientSize.Width+300 , 2*ClientSize.Height);
            radius = Math.Min(2*ClientSize.Width - 50, 2*ClientSize.Height-50);
            angle = 0;
            paint_every();
        }
        private void paint_every()
        {
            try
            {
                if (stop)
                {
                    return;
                }

                CalculateRadarHead();
                graphics = Graphics.FromImage(bwm);
                pen = new Pen(Color.Red, 5);

                // Draw the radar head
                graphics.DrawLine(pen, radarCenter, radarHead);
                distance = Math.Pow(radarCenter.X - radarHead.X, 2) + Math.Pow(radarCenter.Y - radarHead.Y, 2);
                distance = Math.Sqrt(distance);


                pen = new Pen(Color.Green, 5);
                graphics.DrawEllipse(pen, radarCenter.X - (float)distance, radarCenter.Y - (float)distance, (float)distance * 2, (float)distance * 2);//full
                pen = new Pen(Color.Green, 2);
                graphics.DrawEllipse(pen, radarCenter.X - (float)distance / 2, radarCenter.Y - (float)distance / 2, (float)distance, (float)distance);//.5

                graphics.DrawEllipse(pen, radarCenter.X - (float)distance / 4, radarCenter.Y - (float)distance / 4, (float)distance / 2, (float)distance / 2);//.25


                graphics.DrawEllipse(pen, radarCenter.X - (float)distance * (float).75, radarCenter.Y - (float)distance * (float).75, (float)distance * (float)1.5, (float)distance * (float)1.5);
                int startAngle = 0;
                int segmentAngle = 180 / 4;
                List<Label> labels = new List<Label>();
                pen = new Pen(Color.Green, 5);// Divide the half circle into 4 equal segments
                for (int i = -1; i < 4; i++)
                {
                    int lineAngle = startAngle + (i + 1) * segmentAngle;
                    double lineRadians = lineAngle * Math.PI / 180;
                    int lineX = (int)(radarCenter.X + radius * Math.Cos(lineRadians));
                    int lineY = (int)(radarCenter.Y - radius * Math.Sin(lineRadians));
                    graphics.DrawLine(pen, radarCenter.X, radarCenter.Y, lineX, lineY);
                    graphics.DrawString(lineAngle.ToString(), new Font("Arial", 10), Brushes.White, lineX, lineY - 10);
                }

                this.pictureBox1.Image = bwm;
                graphics.Dispose();
                pen.Dispose();
            }
            catch { }
        }

        public PointF DRAW_POINT(float distance , float angle)
        {
            float lol = distance * (float)17.05;
            double lineRadians = angle * Math.PI / 180;
            double x = radarCenter.X + lol * Math.Cos(lineRadians);
            double y = radarCenter.Y - lol* Math.Sin(lineRadians);
            PointF pointn = new PointF((float)x, (float)y);
            return pointn;
        }

        private void pictureBox1_MouseClick(object sender, MouseEventArgs e)
        {
            foreach (Point pointn in drawnPoints)
            {
                if (IsMouseOnDrawnPoint(e.Location, pointn))
                {
                    double lol = Math.Pow(radarCenter.X - pointn.X, 2) + Math.Pow(radarCenter.Y - pointn.Y, 2);
                    double LLLL = (Math.Sqrt(lol))*40/682;
                    double aaaa=Math.Abs(CalculateAngleWithXAxis(pointn.X, pointn.Y));
                    // Display a message box with (distance, angle) of the point
                    MessageBox.Show($"distance = {LLLL.ToString("F2")}\nangle = {aaaa.ToString("F2")}");
                    return;
                }
            }
        }

        private double CalculateAngleWithXAxis(double x1, double y1)
        {
            double angleRadians = Math.Atan2(y1 - radarCenter.Y, x1 - radarCenter.X);
            double angleDegrees = angleRadians * (180.0 / Math.PI);
            return angleDegrees;
        }
        private bool IsMouseOnDrawnPoint(Point mouseLocation, Point point)
        {
            Rectangle pointRect = new Rectangle(point.X - 4, point.Y - 4, 6, 6);
            return pointRect.Contains(mouseLocation);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            stop = !stop;
            if (stop) this.button1.Text = "START";
            else this.button1.Text = "STOP";
            try
            {
                serialPort.Write("0");
            }
            catch (Exception s)
            { 
                MessageBox.Show(s.Message);
            }
       
        }
        private void CalculateRadarHead()
        {
            int x = (int)(radarCenter.X + radius * Math.Cos(angle));
            int y = (int)(radarCenter.Y - radius * Math.Sin(angle));
            radarHead = new Point(x, y);
        }

        private void Form1_Load(object sender, EventArgs e)
        {}
    }

}
