#include "Header.h"

// Define the mouse callback
//void my_mouse_callback(int event, int x, int y, void* param);

Rect box;
bool drawing_box = false;
void draw_box(Mat &img, Rect box);
static void my_mouse_callback(int event, int x, int y,int f, void* param)
{
	Mat& image = *(Mat*)param;
	switch (event)
	{
	case::EVENT_MOUSEMOVE: {
		if (drawing_box)
		{
			box.width = x - box.x;
			box.height = y - box.y;
		}
		break;
	}

	case::EVENT_LBUTTONDOWN:
	{
		drawing_box = true;
		box = Rect(x, y, 0, 0);
	}
	break;

	case::EVENT_LBUTTONUP:
	{
		drawing_box = false;
		if (box.width < 0)
		{
			box.x += box.width;
			box.width += -1;
		}
		if (box.height < 0)
		{
			box.y += box.height;
			box.height += -1;
		}
		draw_box(image, box);
	}
	break;
	}
}
// Function to draw a box onto an image
void draw_box(Mat &img, Rect box)
{
	rectangle(img,box.tl(), box.br(), red);
}

int main()
{
	box = Rect(-1, -1, 0, 0);
	Mat img(200, 200, CV_8UC3), temp;
	img.copyTo(temp);
	img = Scalar::all(0);

	namedWindow("Box example");

	setMouseCallback("Box example", my_mouse_callback, (void*)&img);
	for (;;)
	{
		img.copyTo(temp);
		if (drawing_box)
		{
			draw_box(temp, box);
		}
		imshow("Box example", temp);

		if (waitKey(15) == 27)
			break;
	}
}

