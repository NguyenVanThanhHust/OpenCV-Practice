#include "Header.h"
struct hough_val
{
	// radius and center
	int r;
	int xC;
	int yC;
	int av; //accumulator val
};

bool my_comp(const hough_val hv1, const hough_val hv2)
{
	if (hv1.av != 0 && hv2.av != 0)
	{
		return hv1.av > hv2.av;
	}
	else
		return hv1.av < hv2.av;
}

void HC(Mat img_edge, int minR, int maxR, float thres1, float thres2, Mat &draw)
{
	// Assumption : 
	// Center is inside the image
	// xC = x - R*cos(theta) ;  yC = y - R*sin(theta)

	Mat ie;
	img_edge.convertTo(ie, CV_32F);

	// Step 1 : Create accumulator matrix
	int w = ie.cols, h = ie.rows;
	// Renge of r,xC,yC : r in range minR -> maxR ; xC in range r -> h-r ; yC in range r -> w-r
	const int sizes[] = { 2 * h, 2 * w ,maxR - minR + 2 }; // x center, y center and radius

	Mat accu = Mat(3, sizes, CV_32FC1, Scalar(0));

	// Step 2 : Fill the accumulator
	// For each point in image
	for (int i = 0; i < ie.rows; i++)
	{
		for (int j = 0; j < ie.cols; j++)
		{
			// If the point is on edge
			if (ie.at<float>(i, j) > thres1)
			{
				// Put it into accumulator matrix
				for (int r = 0; r < accu.size[2]; r++)
				{
					for (int theta = 0; theta < 180; theta += 90)
					{
						/*
						int xC = i - (r + minR)*cos(theta*DegreeToRadian); // xC in range r -> h-r
						int yC = j - (r + minR)*sin(theta*DegreeToRadian); // yC in range r -> w-r
						int xCin = xC + minR + r; // xCin in range r + minR -> h - r + minR
						int yCin = yC + minR + r; // yCin in range r + minR -> h - r + minR
						*/
						int xCin = i - (r + minR)*cos(theta*DegreeToRadian) + minR + r;
						int yCin = j - (r + minR)*sin(theta*DegreeToRadian) + minR + r;
						accu.at<float>(xCin, yCin, r)++;
					}
				}
			}
		}
	}
	cout << "3" << endl;
	// Step 3 : Find the local maximums
	vector<hough_val> HS; // Hough Set for local maximum
	for (int i = 1; i < accu.size[0] - 1; i++) // x
	{
		for (int j = 1; j < accu.size[1] - 1; j++) // y
		{
			for (int k = 1; k < accu.size[2] - 1; k++) // r
			{
				// Take only value > threshold 2
				if (accu.at<float>(i, j, k) > thres2)
				{
					for (int a = 0; a < 3; a++)
					{
						for (int b = 0; b < 3; b++)
						{
							for (int c = 0; c < 3; c++)
							{
								int xn = i + a - 1, yn = j + b - 1, zn = k + c - 1;
								if (accu.at<float>(xn, yn, zn) > accu.at<float>(i, j, k))
								{
									// Just ignore
								}
								else
								{
									hough_val tem_h;
									tem_h.r = k + minR;
									tem_h.xC = i + tem_h.r;
									tem_h.yC = j + tem_h.r;
									tem_h.av = accu.at<float>(i, j, k);
									HS.push_back(tem_h);
								}
							}
						}
					}
				}
			}
		}
	}
	cout << "4" << endl;
	// Step 4 : Check how much accumulator value we need to use to draw line
	// Delete all the duplicates element in set and erase them
	set<int> tempSet; // Temp set for accu value

					  // partition the elements, unique items on left, duplicates on right
	auto divider = stable_partition(HS.begin(), HS.end(), [&](const hough_val& hs)
	{
		// return true if item is not in the set, false otherwise
		return tempSet.insert(hs.av).second;
	});
	cout << "5" << endl;
	// erase them now
	HS.erase(divider, HS.end());

	// Sort the value of Hough Set
	std::sort(HS.begin(), HS.end(), my_comp);

	HS.pop_back();
	// Step 5 : Draw the circle 
	while (!HS.empty())
	{
		hough_val tem_h = HS.at(HS.size() - 1);
		int R = tem_h.r;
		int x1 = tem_h.xC - minR;
		int y1 = tem_h.yC - minR;
		cout << x1 << "  " << y1 << " " << R << endl;
		circle(draw, Point(x1, y1), R, red, 1);
		circle(draw, Point(x1, y1), 5, red, 1);
		HS.pop_back();
	}

	imshow("result", draw);
}

int main()
{
	Mat new_img = imread("aaa.jpg", 1);

	Mat img_edge;
	Canny(new_img, img_edge, 150, 50);
	//imshow("cannny", img_edge);
	HC(img_edge, 220, 240, 120, 0, new_img);

	cout << "done" << endl;
	waitKey(0);
	return 0;
}