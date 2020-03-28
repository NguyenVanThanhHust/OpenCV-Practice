#include "Header.h"

struct hough_val
{
	int rho;
	int theta;
	int av; //accumulator value
};

// Compare function for sort
bool my_comp(const hough_val hv1, const hough_val hv2)
{
	if (hv1.av != 0 && hv2.av != 0)
	{
		return hv1.av > hv2.av;
	}
	else
		return hv1.av < hv2.av;
}

void HL(Mat img_edge, float th1, float th2, Mat &draw, int NoL)
{

	Mat ie;
	img_edge.convertTo(ie, CV_32F);
	Mat ic = draw;
	// Step 1 : Create accumulator matrix
	int w = ie.cols, h = ie.rows;
	int D = int(sqrt(w*w + h*h));
	int accu_w = 180 + 2, accu_h = 2 * D + 2;
	Mat accu = Mat::zeros(Size(accu_w, accu_h), CV_32F);

	// Step 2 : Fill the accumulator
	for (int i = 0; i < ie.rows; i++)
	{
		for (int j = 0; j < ie.cols; j++)
		{
			if (ie.at<float>(i, j) > th1)
			{
				// For theta from 0 to 180 
				for (int theta = 0; theta < 180; theta += 2)
				{
					int rho = (int)(i*sin(theta*DegreeToRadian) + j*cos(theta*DegreeToRadian));
					int r = rho + D;
					accu.at<float>(r, theta)++;
				}
			}
		}
	}

	// Step 3 : Find the local maximums
	vector<hough_val> HS; // Hough Set for local maximum
	for (int r = 1; r < accu.rows - 1; r++) // r = rho
	{
		for (int a = 1; a < accu.cols - 1; a++) // a = angle 
		{
			// Take only value > threshold 2
			if (accu.at<float>(r, a) > th2)
			{
				for (int b = 0; b < 3; b++)
				{
					for (int c = 0; c < 3; c++)
					{
						int xn = r + b - 1, yn = a + c - 1;
						if (accu.at<float>(xn, yn) > accu.at<float>(r, a))
						{
							// Just ignore 
						}
						else
						{
							if (r - D > 0)
							{
								hough_val tem_h;
								tem_h.rho = r - D;
								tem_h.theta = a;
								tem_h.av = accu.at<float>(r, a);
								HS.push_back(tem_h);
							}
						}
					}
				}
			}
		}
	}

	// Step 4 : Check how much accumulator value we need to use to draw line
	// Delete all the duplicates element in set and erase them
	set<int> tempSet; // Temp set for accu value
					  // partition the elements, unique items on left, duplicates on right
	auto divider = stable_partition(HS.begin(), HS.end(), [&](const hough_val& hs)
	{
		// return true if item is not in the set, false otherwise
		return tempSet.insert(hs.av).second;
	});

	// erase them now
	HS.erase(divider, HS.end());

	// Sort the value of Hough Set
	sort(HS.begin(), HS.end(), my_comp);

	int NL; // Number of line in real problem
	if (HS.size() > NoL)
	{
		HS.erase(HS.begin() + NoL, HS.end());
		NL = NoL;
	}
	else
	{
		NL = HS.size();
	}

	// Step 5 : Draw the line
	while (!HS.empty())
	{
		hough_val tem_h = HS.at(HS.size() - 1);
		int t = tem_h.theta;
		int r = tem_h.rho;
		int x1, y1, x2, y2;
		x1 = y1 = x2 = y2 = 0;
		if (t > 45 && t < 135)
		{
			//y = (r - x cos(t)) / sin(t)  
			x1 = 0;
			y1 = (double)((r - (x1* cos(t * DegreeToRadian))) / sin(t * DegreeToRadian));
			x2 = h;
			y2 = (double)((r - (x2* cos(t * DegreeToRadian))) / sin(t * DegreeToRadian));
		}
		else
		{
			//x = (r - y sin(t)) / cos(t);  
			y1 = 0;
			x1 = (double)((r - y1 * sin(t * DegreeToRadian)) / cos(t * DegreeToRadian));
			y2 = w;
			x2 = (double)((r - y2 * sin(t * DegreeToRadian)) / cos(t * DegreeToRadian));
		}
		Point p1, p2;
		p1.x = (int)x1, p1.y = (int)y1;
		p2.x = (int)x2, p2.y = (int)y2;
		line(ic, p1, p2, red);
		HS.pop_back();
	}

	imshow("result", ic);
}

// Fucntion to calculate 
int main()
{
	Mat new_img = imread("patheon.jpg", 1);
	Mat img_edge;
	Canny(new_img, img_edge, 150, 50);
	HL(img_edge, 150, 40, new_img, 10);
	waitKey(0);
	return 0;
}