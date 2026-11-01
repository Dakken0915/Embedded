---
title: OpenCV
---

# OpenCV 图像处理

本文使用 **OpenCV 4.6、C++17、Linux** 接口，依赖 `core`、`imgproc`、`imgcodecs`、`videoio`、`highgui`、`features2d`、`calib3d`、`video` 模块。窗口示例要求可用的 GUI 后端和显示环境；无显示设备的嵌入式系统可保留处理与写文件部分。

除完整程序外，代码块为独立片段，统一使用以下声明；`img` 表示非空 `CV_8UC3` BGR 图像，`gray` 表示对应的非空 `CV_8UC1` 灰度图。输出变量在各片段中声明。

```cpp
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using std::vector;
```

## 图像链路与像素格式

```text
摄像头采集 → 解码或像素格式转换 → 缩放/ROI → 算法处理 → 结果叠加 → 显示或传输
```

输入契约应明确分辨率、像素格式、行跨度、帧率、颜色矩阵/量化范围以及缓冲区生命周期。YUYV 是打包 YUV 4:2:2，NV12 是 Y 平面加交错 UV 平面的 YUV 4:2:0；MJPEG 是压缩帧，需要解码。不能将这些原始数据直接当作三通道 BGR。

原始 YUYV 可表示为 `CV_8UC2`，再用 `COLOR_YUV2BGR_YUY2` 转换；输入宽度须为偶数，构造矩阵时传入真实 stride，并确保缓冲区容量覆盖全部行。NV12 转换通常要求偶数宽高及正确的平面布局，可使用 `cvtColorTwoPlane`；MJPEG 缓冲区可交给 `imdecode`。转换结果的颜色是否正确还取决于源数据的颜色标准。具体采集缓冲所有权见 [V4L2 视频采集与缓冲区](../../Linux应用/V4L2视频采集.md)。

许多算法直接接收灰度图，不必统一先转为 RGB；默认彩色 `imread` 与常见 `VideoCapture` 解码输出为 BGR，采集后端特殊模式仍须单独确认。

## Mat、坐标与内存所有权

`Mat` 保存尺寸、类型、数据指针及 `step` 等信息；`CV_8UC3` 只描述 8 位无符号三通道，不自动规定 BGR/RGB 语义。`Point(x,y)` 使用横纵坐标，像素访问使用 `at<T>(y,x)`；`Size(width,height)` 与 `Mat(rows,cols,type)` 参数顺序不同。

| 类型或常量 | 含义 |
| --- | --- |
| `CV_8UC1`、`CV_8UC3` | 8 位无符号单通道、三通道 |
| `CV_16SC1`、`CV_32FC1` | 16 位有符号、32 位浮点单通道 |
| `Point`、`Point2f` | 整数点、浮点点 |
| `Size`、`Rect` | 尺寸、左上角加宽高的矩形 |
| `Scalar`、`Vec3b` | 多分量标量、三个字节的向量 |
| `RotatedRect` | 中心、尺寸和角度表示的旋转矩形 |

```cpp
Mat zeros = Mat::zeros(480, 640, CV_8UC3);
Mat ones = Mat::ones(480, 640, CV_8UC1);
Mat red(480, 640, CV_8UC3, Scalar(0, 0, 255));
Mat shared = red;             // 共享像素存储，修改会影响 red
Mat independent = red.clone();
Mat copied;
red.copyTo(copied);           // 复制像素；目标可复用已有分配
red.at<Vec3b>(20, 10) = Vec3b(255, 0, 0);
Vec3b* row = red.ptr<Vec3b>(20);
row[11] = Vec3b(0, 255, 0);   // 行、列索引必须在范围内
RotatedRect box(Point2f(100, 100), Size2f(100, 50), 30);
```

ROI 默认也共享存储，矩形必须落在图像范围内；非连续 ROI 不能按 `rows * cols * elemSize()` 随意线性遍历。

```cpp
Rect requested(100, 100, 200, 200);
Rect clipped = requested & Rect(0, 0, img.cols, img.rows);
if (clipped.area() > 0) {
    Mat roi = img(clipped);
    Mat owned = roi.clone();  // 需要独立生命周期或快照时复制
    (void)owned;
}
```

由外部指针构造的 `Mat` 不接管该内存的释放。V4L2 缓冲区重新入队后不能继续读取旧视图；跨线程传递共享 `Mat` 也不等于像素读写具备同步保护。相关接口见 [Mat 官方参考](https://docs.opencv.org/4.6.0/d3/d63/classcv_1_1Mat.html)。

## 图像读写与窗口

`imread` 失败返回空矩阵，`imwrite` 返回成功标志，也可能抛出 `cv::Exception`。文件扩展名决定编码格式；JPEG 是有损编码，不适合要求像素完全一致的中间结果。

以下为完整程序，输入与输出路径来自参数，不依赖个人机器的绝对路径。

```cpp
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: image_io input output\n";
        return 1;
    }
    try {
        cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);
        if (img.empty()) {
            std::cerr << "cannot decode input image\n";
            return 1;
        }
        if (!cv::imwrite(argv[2], img)) return 1;
        cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
        cv::imshow("image", img);
        cv::waitKey(0);
        cv::destroyAllWindows();
    } catch (const cv::Exception& e) {
        cv::destroyAllWindows();
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
```

Linux 安装开发包并提供 `opencv4.pc` 后，保存为 `image_io.cpp` 可编译运行：

```bash
c++ -std=c++17 -Wall -Wextra image_io.cpp -o image_io $(pkg-config --cflags --libs opencv4)
./image_io input.jpg output.png
```

灰度读取使用 `imread(path, IMREAD_GRAYSCALE)`，保留深度和 alpha 使用 `IMREAD_UNCHANGED`，后者可能产生四通道或 16 位图像。`imshow` 后需要 `waitKey` 或 `pollKey` 处理窗口事件；`waitKey(0)` 阻塞等待按键，正数参数是等待时间下限，不能作为精确帧率控制器。

## 视频采集、文件播放与写入

C++ 的 `bool VideoCapture::read(OutputArray image)` 返回成功标志，通过参数写入图像；Python 的 `(ret, frame)` 返回形式不能直接用于 C++。`read` 组合了 `grab` 与 `retrieve`，后两者分别用于抓取及取回/解码；同一轮不能先 `read` 又 `grab`，否则通常会多推进一帧。

属性设置经过后端、驱动和设备，`set` 返回成功也不能保证精确采用请求值；应检查返回值并通过 `get`、实际图像尺寸和时间戳验证。`CAP_PROP_BUFFERSIZE=1` 并非所有后端都支持，也不保证整个链路只有一帧延迟。见 [VideoCapture 官方参考](https://docs.opencv.org/4.6.0/d8/dfe/classcv_1_1VideoCapture.html)。

下面的完整程序使用 Linux V4L2，显示并写出视频。GUI、摄像头和 MJPG/AVI 编码后端必须可用；25 FPS 是输出时间基准，不是对采集速率的测量值。

```cpp
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    try {
        cv::VideoCapture cap(0, cv::CAP_V4L2);
        if (!cap.isOpened()) return 1;
        const bool format_ok = cap.set(cv::CAP_PROP_FOURCC,
            cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        const bool width_ok = cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        const bool height_ok = cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        const bool fps_ok = cap.set(cv::CAP_PROP_FPS, 25);
        if (!(format_ok && width_ok && height_ok && fps_ok))
            std::cerr << "some capture properties were rejected\n";
        std::cerr << "reported FPS: " << cap.get(cv::CAP_PROP_FPS) << '\n';
        cv::Mat frame;
        if (!cap.read(frame) || frame.empty() || frame.type() != CV_8UC3)
            return 1;
        cv::VideoWriter writer("out.avi",
            cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
            25.0, frame.size(), true);
        if (!writer.isOpened()) return 1;
        const cv::Size expected = frame.size();
        cv::namedWindow("video", cv::WINDOW_AUTOSIZE);
        do {
            if (frame.empty() || frame.size() != expected ||
                frame.type() != CV_8UC3) return 1;
            writer.write(frame);
            cv::imshow("video", frame);
            if ((cv::waitKey(1) & 0xff) == 'q') break;
        } while (cap.read(frame));
        writer.release();   // 完成容器写入；析构也会释放
        cap.release();
        cv::destroyAllWindows();
    } catch (const cv::Exception& e) {
        cv::destroyAllWindows();
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
```

播放文件使用 `VideoCapture cap("input.mp4")`；RTSP 使用流地址，其协议与解码支持取决于 FFmpeg/GStreamer 等后端。文件读到末尾与设备断开都可能表现为 `read` 失败，需要由上层业务区分；网络重连应设置有限重试与超时，避免无限阻塞。

写入帧的尺寸、通道与深度须符合编码器要求。`mp4v` 可用于部分 MP4 后端，但 FourCC 不是编码器可用性的保证。`VideoWriter::write` 没有逐帧布尔返回值，打开检查、异常捕获以及写后文件验证均有必要。固定 FPS 容器不会根据实际采集时间自动调整每帧时间戳。

## 通道与颜色空间

```cpp
vector<Mat> channels;
split(img, channels);
Mat merged, gray, hsv, rgb;
merge(channels, merged);
cvtColor(img, gray, COLOR_BGR2GRAY);
cvtColor(img, hsv, COLOR_BGR2HSV);
cvtColor(img, rgb, COLOR_BGR2RGB);
```

8 位标准 HSV 中 H 通常为 0～179，S、V 为 0～255；浮点图像与 `HSV_FULL` 采用不同范围。颜色阈值必须与转换模式一致。灰度转换会丢失色彩信息，不能再精确还原原始彩色图。

## 滤波与降噪

均值和高斯滤波平滑局部噪声，也会削弱边缘；中值滤波适合脉冲噪声。双边滤波综合空间与颜色距离以减少跨边缘混合，通常比普通平滑耗时。

```cpp
Mat mean, gaussian, median, bilateral, sharpened;
blur(img, mean, Size(3, 3));
GaussianBlur(img, gaussian, Size(5, 5), 0);
medianBlur(img, median, 5);
bilateralFilter(img, bilateral, 15, 80, 80);
Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
filter2D(img, sharpened, -1, kernel);
```

高斯显式核尺寸和中值孔径应满足接口要求，中值孔径为大于 1 的奇数；支持的深度还取决于孔径。`filter2D` 实际执行相关运算，不翻转核；需要数学卷积时须先翻转核。`ddepth=-1` 保留输入深度，8 位结果会饱和截断；需要保留负值或较大动态范围时先输出到浮点矩阵。

## 几何变换

缩小图像可使用 `INTER_AREA`；放大常用 `INTER_LINEAR` 或 `INTER_CUBIC`。保持宽高比需要自行计算输出尺寸，直接指定固定宽高可能拉伸图像。

```cpp
Mat scaled, half, rotated, flipped, affine, perspective;
resize(img, scaled, Size(320, 240), 0, 0, INTER_AREA);
// 输入宽高至少为 2，避免比例缩放后出现零尺寸
if (img.cols >= 2 && img.rows >= 2)
    resize(img, half, Size(), 0.5, 0.5, INTER_AREA);
Point2f center((img.cols - 1) * 0.5f, (img.rows - 1) * 0.5f);
Mat rotation = getRotationMatrix2D(center, 45.0, 1.0);
warpAffine(img, rotated, rotation, img.size());
flip(img, flipped, 0);   // 上下；1 为左右，-1 为两轴同时翻转

vector<Point2f> src{{0, 0}, {100, 0}, {0, 100}};
vector<Point2f> dst{{10, 10}, {110, 20}, {20, 110}};
Mat transform = getAffineTransform(src, dst);
warpAffine(img, affine, transform, img.size());

vector<Point2f> src4{{0, 0}, {100, 0}, {100, 100}, {0, 100}};
vector<Point2f> dst4{{10, 10}, {110, 20}, {100, 110}, {20, 100}};
Mat inlier_mask;
Mat homography = findHomography(src4, dst4, RANSAC, 3.0, inlier_mask);
if (!homography.empty())
    warpPerspective(img, perspective, homography, img.size());
```

仿射矩阵为 2×3，需要三对非共线点；单应矩阵为 3×3，至少需要四对位置非退化的对应点。RANSAC 的内点掩码用于检验匹配支持程度，不能只凭返回非空矩阵判断变换可靠。输出画布与原图同大时，旋转和透视结果可能被裁切；边界填充和输出范围需要显式设计。见 [仿射变换说明](https://docs.opencv.org/4.6.0/d4/d61/tutorial_warp_affine.html)。

## 梯度、边缘与形态学

Sobel 近似一阶导数，Laplacian 近似二阶导数；导数具有正负值，不能直接使用 `CV_8U` 保存完整结果。`convertScaleAbs` 取绝对值并转换到 8 位，适合显示，不再保留方向符号。

```cpp
Mat smooth, edges, gx, gy, lap, abs_x, abs_y, gradient, lap_view;
GaussianBlur(gray, smooth, Size(5, 5), 0);
Canny(smooth, edges, 50, 150);
Sobel(gray, gx, CV_16S, 1, 0, 3);
Sobel(gray, gy, CV_16S, 0, 1, 3);
Laplacian(gray, lap, CV_16S, 3);
convertScaleAbs(gx, abs_x);
convertScaleAbs(gy, abs_y);
addWeighted(abs_x, 0.5, abs_y, 0.5, 0, gradient);
convertScaleAbs(lap, lap_view);
```

二值图以非零像素作为前景。腐蚀缩小前景、膨胀扩大前景；开运算先腐蚀后膨胀，闭运算顺序相反；形态学梯度为膨胀结果与腐蚀结果之差。核尺寸、形状、迭代次数会改变目标连接关系。

```cpp
Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
Mat eroded, dilated, opened, closed, morph_gradient;
erode(gray, eroded, kernel);
dilate(gray, dilated, kernel);
morphologyEx(gray, opened, MORPH_OPEN, kernel);
morphologyEx(gray, closed, MORPH_CLOSE, kernel);
morphologyEx(gray, morph_gradient, MORPH_GRADIENT, kernel);
```

## 阈值、轮廓与形状

固定阈值对整幅图使用同一阈值；Otsu 从整幅图直方图选择一个全局阈值，属于自动全局阈值法。`adaptiveThreshold` 按局部邻域计算阈值，适合光照变化；不能将两者混称为同一算法。见 [阈值处理官方说明](https://docs.opencv.org/4.6.0/d7/d4d/tutorial_py_thresholding.html)。

```cpp
Mat binary, otsu, adaptive;
threshold(gray, binary, 128, 255, THRESH_BINARY);
double selected = threshold(gray, otsu, 0, 255, THRESH_BINARY | THRESH_OTSU);
adaptiveThreshold(gray, adaptive, 255, ADAPTIVE_THRESH_GAUSSIAN_C,
                  THRESH_BINARY, 11, 2);
(void)selected;
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;
findContours(otsu, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
Mat overlay = img.clone();
drawContours(overlay, contours, -1, Scalar(0, 255, 0), 2);
if (!contours.empty() && !contours.front().empty()) {
    const auto& contour = contours.front();
    vector<Point> approx;
    approxPolyDP(contour, approx, 0.02 * arcLength(contour, true), true);
    Rect bounds = boundingRect(contour);
    rectangle(overlay, bounds, Scalar(255, 0, 0), 2);
    Point2f center;
    float radius = 0;
    minEnclosingCircle(contour, center, radius);
}
```

此处输入统一为 8 位单通道；自适应阈值邻域大小必须是大于 1 的奇数。`RETR_EXTERNAL` 只保留外层轮廓，需要孔洞关系时使用 `RETR_TREE` 等模式。层级每项依次为 next、previous、first-child、parent 的索引，缺失项为 -1。`CHAIN_APPROX_SIMPLE` 压缩直线段；轮廓列表不保证按面积或位置排序，业务目标应按面积、周长或形状筛选，不能假定第一个就是最大目标。

## 直线、圆、角点与局部特征

```cpp
vector<Vec3f> circles;
Mat smoothed;
medianBlur(gray, smoothed, 5);
HoughCircles(smoothed, circles, HOUGH_GRADIENT, 1, 50, 100, 30, 10, 100);
Mat edges;
Canny(gray, edges, 50, 150);
vector<Vec4i> lines;
HoughLinesP(edges, lines, 1, CV_PI / 180, 80, 30, 10);
Mat harris;
cornerHarris(gray, harris, 2, 3, 0.04);
Ptr<SIFT> sift = SIFT::create();
vector<KeyPoint> keypoints;
Mat descriptors, feature_view;
sift->detectAndCompute(gray, noArray(), keypoints, descriptors);
drawKeypoints(img, keypoints, feature_view);
```

Hough 圆结果为 `(x,y,radius)`，概率 Hough 直线结果为 `(x1,y1,x2,y2)`。圆检测中的 `minDist`、边缘阈值、累加器阈值和半径范围须按实际尺度配置；Harris 输出为浮点响应图，需要阈值与局部极大值筛选才能获得离散角点。

本文采用的 OpenCV 4.6 中，`cv::SIFT` 已位于主库 `features2d`，不要求 contrib。SURF 属于 `xfeatures2d` 扩展模块，需匹配版本的 opencv_contrib，并在构建时启用 `OPENCV_ENABLE_NONFREE`；发行版不一定提供。见 [SIFT 接口](https://docs.opencv.org/4.6.0/d7/d60/classcv_1_1SIFT.html) 和 [SURF 说明](https://docs.opencv.org/4.6.0/df/dd2/tutorial_py_surf_intro.html)。

## 直方图与对比度

```cpp
int bins = 256;
int channel = 0;
float range[] = {0, 256};
const float* ranges[] = {range};
Mat hist, equalized, local_equalized;
calcHist(&gray, 1, &channel, Mat(), hist, 1, &bins, ranges);
equalizeHist(gray, equalized);
Ptr<CLAHE> clahe = createCLAHE(2.0, Size(8, 8));
clahe->apply(gray, local_equalized);
Mat second_hist = hist.clone(); // 实际比较时替换为另一幅图的同规格直方图
normalize(hist, hist, 1.0, 0, NORM_L1);
normalize(second_hist, second_hist, 1.0, 0, NORM_L1);
double similarity = compareHist(hist, second_hist, HISTCMP_CORREL);
(void)similarity;
```

`[0,256)` 覆盖 8 位全部灰度值，`calcHist` 的通道、维数、bin 数和范围必须一致。全局均衡化可能放大噪声；CLAHE 对分块直方图限制对比度，`Size(8,8)` 表示网格数量。彩色图可在亮度通道处理，分别均衡 B、G、R 容易改变色彩。直方图相关性不保留空间结构，即使分布接近也不能证明内容相同。

## 图形与文本叠加

```cpp
Mat canvas = img.clone();
line(canvas, Point(0, 0), Point(100, 100), Scalar(0, 0, 255), 2);
rectangle(canvas, Rect(10, 10, 100, 100), Scalar(0, 255, 0), 2);
circle(canvas, Point(100, 100), 50, Scalar(255, 0, 0), FILLED);
ellipse(canvas, Point(100, 100), Size(100, 50), 0, 0, 360,
        Scalar(255, 0, 0), 2);
putText(canvas, "Hello", Point(10, 30), FONT_HERSHEY_SIMPLEX,
        1.0, Scalar(255, 255, 255), 2, LINE_AA);
```

颜色按目标矩阵的通道语义解释，`FILLED` 或厚度 -1 表示填充封闭图形。`putText` 的原点通常是文字基线左端，内置 Hershey 字体不直接支持完整中文字符；中文需另用字体渲染模块。

## 帧差、背景减除与光流

以下片段要求 `frame1`、`frame2` 为同尺寸、同类型 BGR 图像；`prev_gray`、`next_gray` 为相邻的同尺寸 8 位灰度帧。

```cpp
Mat diff, motion;
absdiff(frame1, frame2, diff);
cvtColor(diff, diff, COLOR_BGR2GRAY);
threshold(diff, motion, 25, 255, THRESH_BINARY);
```

帧差依赖相邻帧变化，容易受摄像头抖动和光照影响。MOG2 维护背景模型，必须在循环外创建对象，在每帧到来时持续更新：

```cpp
Ptr<BackgroundSubtractorMOG2> bg = createBackgroundSubtractorMOG2();
// 以下调用置于逐帧循环中；此处以 img 表示当前 BGR 帧
Mat fgmask, foreground;
bg->apply(img, fgmask);
threshold(fgmask, foreground, 200, 255, THRESH_BINARY);
```

启用阴影检测时，默认掩码中的 127 通常表示阴影，255 表示前景；按大于 200 二值化可排除默认阴影值。训练初期、曝光突变和镜头运动可能产生大面积误报，学习率控制背景适应速度。

Lucas–Kanade 稀疏光流只跟踪给定特征点，需检查每个点的 `status`，过滤失败或出界点；失败点的 `err` 不应作为有效误差使用。

```cpp
vector<Point2f> prev_pts, next_pts, valid_pts;
goodFeaturesToTrack(prev_gray, prev_pts, 100, 0.3, 7);
if (!prev_pts.empty()) {
    vector<uchar> status;
    vector<float> err;
    calcOpticalFlowPyrLK(prev_gray, next_gray, prev_pts, next_pts, status, err);
    for (size_t i = 0; i < next_pts.size(); ++i) {
        const Point2f p = next_pts[i];
        if (status[i] && p.x >= 0 && p.y >= 0 &&
            p.x < next_gray.cols && p.y < next_gray.rows && err[i] < 20.0f)
            valid_pts.push_back(p);
    }
}
```

误差阈值依赖窗口与成像条件，示例值不能当作通用质量保证。长期跟踪还需补充特征、检测遮挡和剔除错误匹配；更新前一帧时，应使用 `copyTo` 或确保缓冲区不会被采集覆盖。接口约束见 [视频跟踪参考](https://docs.opencv.org/4.6.0/dc/d6b/group__video__track.html)。

## 检测链路与性能边界

人脸检测可采用如下链路：

```text
V4L2 YUYV/MJPEG → 转换/解码 → 缩放/ROI → 检测器 → 坐标还原 → 结果叠加 → LCD/HTTP
```

模型路径、输入尺寸、颜色顺序、归一化方式、置信度阈值和检测周期必须与模型契约一致。ROI 与缩放改变坐标系，检测框需按缩放比例和 ROI 原点还原后再绘制。采集空帧、模型加载失败和异常输入须作为明确错误路径处理。

内存搬运、颜色转换和编解码可能占据主要耗时，不能只依据算法复杂度评估性能。复用输出矩阵、减少无效分辨率和非必要 `split/merge` 可降低带宽压力；需要独立快照时仍须复制。分别记录采集、转换、推理、叠加和传输耗时，并区分处理 FPS 与端到端延迟。

跨线程帧队列应有容量与丢帧策略，生产速率持续高于消费速率时，无界排队会使画面越来越旧。线程退出时先停止生产、结束消费，再释放采集设备和共享缓冲；普通 `Mat`、`Ptr` 及视频对象由 RAII 管理，但外部 DMA/MMAP 存储和线程同步仍由应用负责。
