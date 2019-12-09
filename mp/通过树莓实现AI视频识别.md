
## 由一个实际问题引发的思考

渣土车在城市运输过程中，存在抛洒滴漏的情况，对城市环境造成很大污染。如下图所示，滴漏的渣土会导致飞尘影响空气质量，还需要环卫工人上路清扫。

![eda9be3227363d740e85b7305.jpg](https://bce.bdstatic.com/doc/undefined/eda9be3227363d740e85b7305_eda9be3.jpg)

有些严重的渣土抛洒滴漏问题，还会产生交通安全事故(http://news.jstv.com/a/20171114/1510664243313.shtml)。如下图所示，南京出现了首例因渣土抛洒导致交通事故，货车司机被公诉的案例。

![pic2.png](https://bce.bdstatic.com/doc/undefined/pic2_c0d0cf6.png)

渣土抛洒是城市环卫管理的一个典型难题，主要体现在以下几个方面：
- 渣土抛洒发现不及时，大多数情况需要等到路过的群众举报才得以知道具体地点出现了渣土抛洒。
- 城市道路众多，无法通过实时监视所有道路的方式去发现渣土抛洒问题。
- 渣土运输车辆多，无法通过云端实时分析**车辆上传的道路视频**去发现渣土抛洒问题，云端计算处理的视频数据量过于庞大，成本也很高。

有没有一个方案，能够让车辆自己**主动发现、主动报告**渣土抛洒问题。

<br>

## 基于边缘视频AI的解决方案

上述提到的问题，是一个典型的边缘视频AI的场景，通过边缘视频AI，可以快速发现、定位抛洒问题，并协同城市管理单位进行及时处理。可以考虑通过以下步骤来解决：

1. 模型训练
    - 通过AI模型训练平台生成 **渣土抛洒识别模型** ，目前市面上的AI模型训练平台有很多，这里推荐两个平台
        - 百度AI大脑的EasyDL平台（https://ai.baidu.com/easydl/）
        - 微软的认知服务平台（https://www.customvision.ai/）
2. 边缘环境搭建
    - 在渣土车尾部安装视频摄像头
    - 在渣土车上安装边缘设备**AI-BOX**，并与车尾摄像头连接。AI-BOX可以使用百度AI市场上的 *EdgeBoard边缘AI计算盒*(https://aim.baidu.com/product/7040880d-4d04-4667-97b7-ecafdf9b7609)
3. 边缘设备接入BIE
    - 边缘设备部署Baetyl边缘计算框架，并连接百度智能云，可以参考 *快速上手智能边缘*(https://cloud.baidu.com/doc/BIE/s/Gk2b8fwvz)
4. 云端配置边缘应用模块，各模块在边缘设备上实现以下功能：
    - 接入视频摄像头数据
    - 使用渣土抛洒模型对摄像头数据执行AI推断，判断是否有渣土抛洒
    - 如果有抛洒，则保存抛洒图片，并发送一条消息通知消息说明抛洒情况

<br>

## 模拟场景

前面描述的 **渣土抛洒** 所使用的**渣土抛洒识别模型**无法对外公开，本文将使用已经在github开源的 *ssd_mobilenet_v1_coco_2017_11_17* 作为替代模型，完整演示以下的模拟场景

1. 摄像头连接树莓派，实时探测视野范围内的物体
2. 当检测到目标物体以后，保存抽帧图像，并同步发送一条消息到边缘hub模块。如果没有检测到目标物体，丢弃抽帧图像
3. 支持检测多目标物体，本场景实验检测的物体包括：`剪刀`、`笔记本电脑`、`书本`、`键盘`和`人`。


## 前提准备
- 有一个百度智能云账号
- 有一个树莓派，本实验用的是 [Raspberry Pi 4](https://www.raspberrypi.org/products/raspberry-pi-4-model-b/specifications/)，也可以使用树莓派的其他型号([Raspberry Pi 3 Model B+](https://www.raspberrypi.org/products/raspberry-pi-3-model-b-plus/))，或者使用Ubuntu16.04的虚拟机
- 有一个摄像头，本实验用的是 [Raspberry Pi Camera Module v2](https://www.raspberrypi.org/products/camera-module-v2/)，也可以使用其他[USB摄像头](https://item.jd.com/31856488610.html)或者[IP摄像头](https://item.jd.com/1496629.html)
- 有一个物体识别模型，本实验用的是 *ssd_mobilenet_v1_coco_2017_11_17*(https://doc.bce.baidu.com/bce-documentation/BIE/ssd_mobilenet_v1_coco_2017_11_17.zip)，该模型支检测90种物体，具体清单请参考：*mscoco_label_map*(https://github.com/tensorflow/models/blob/master/research/object_detection/data/mscoco_label_map.pbtxt)
- 树莓派已连接云端BIE




<br>

## 通过本教程你将学习
1. 搭建边缘AI硬件环境
2. 配置边缘AI模块
3. 验证边缘AI检测结果

<br>

## 操作流程

### 搭建边缘AI硬件环境

摄像头可以使用`树莓派摄像头模块`、`USB摄像头`、`IP摄像头`，不同的摄像头有不同的配置方式：

- 树莓派摄像头模块：针对树莓派摄像头模块的环境搭建，可以参考 [树莓派3B摄像头安装与测试](https://github.com/PaddlePaddle/Paddle-Lite-Demo/blob/master/PaddleLite-armlinux-demo/enable-camera-on-raspberry-pi.md)

- USB摄像头：针对USB摄像头的环境搭建，请参考官方文档 [使用标准USB摄像头](https://www.raspberrypi.org/documentation/usage/webcams/)。
   
   > 默认情况下，在树莓派摄像头模块和USB摄像头连接树莓派以后，系统中会新增了 `/dev/video0` 这个设备，这个参数`0`将在后续的 [video-infer模块配置](#模块4：video-infer模块) 当中被用到。
   
- IP摄像头：树莓派需要通过 `rtsp` 协议访问IP摄像头，可以先通过 [VLC media player](https://www.videolan.org/vlc/) 等工具测试IP摄像头是否可以通过 `rtsp` 协议访问。如下图所示：

    ![vlc1.png](https://bce.bdstatic.com/doc/undefined/vlc1_37bc8df.png)

    ![vlc2.png](https://bce.bdstatic.com/doc/undefined/vlc2_e8cb38b.png)

   > rtsp协议地址通用格式为 `rtsp://<username>:<password>@<ip>:<port>/<Streaming/channels/stream_number>` ，各参数解释如下:
   >    - `<username>` ：摄像头登录用户名 ，一般可以在摄像头底座当中找到
   >    - `<password>` ：摄像头登录密码，一般可以在摄像头底座当中找到
   >    - `<ip>` ：路由器/交换机分配给摄像头的IP地址
   >    - `<port>` ： RTSP 协议的端口号，一般默认为 `554`，
   >    - `<Streaming/channels/stream_number>` ：摄像头信道

### 配置边缘AI模块





除了树莓派初始连接BIE建立的agent模块之外，还需要为树莓派这个边缘核心添加额外4个模块：

| 序号   | 模块名             |  模块类型                  | 
| :---- |  :----            | :----                     | :----                     |
| 1     | agent             | baetyl-agent              | 边缘核心与云端BIE通讯模块           
| 2     | localhub          | baetyl-hub                | 边缘核心本地MQTT Broker消息模块              
| 3     | function-manager  | baetyl-function-manager   | 边缘函数管理模块
| 4     | function-python   | baetyl-function-python36  | 后处理函数，处理AI推断结果             
| 5     | video-infer       | baetyl-video-infer        | AI推断模块，加载AI模型执行AI推断

每个模块都需要做相应的配置

#### 模块关系图

各模块之间的调用关系如下图所示：

![module-arch.png](https://bce.bdstatic.com/doc/BIE/module-arch_aea11fe.png)

#### 模块功能详细说明

1. `video-infer`
    - 采集视频数据
    - 对视频抽帧
    - 基于AI模型对抽帧图像进行AI推断
    - 通过GRPC接口调用后处理函数管理器`function-manager`
2. `function-manger`
    - 接收video-infer模块的调用
    - 为video-infer分配具体的后处理函数`function-python`
3. `function-python`
    - 真实的后处理函数，对video-infer模块的AI推断结果进行后处理
    - 返回后处理结果给`video-infer`模块
4. `video-infer`：
    - 根据后处理函数的返回结果，决定是否保存抽帧图像，是否发送mqtt消息到hub模块


<br>

#### 模块详细配置

模块详细配置请点击“查看原文”(https://cloud.baidu.com/doc/BIE/s/Dk3nv5gum)。


### 验证边缘AI检测结果

#### step1：配置下发

云端一切配置完毕，将配置发布为正式版本，下发至边缘核心设备，并检查模块已经正常启动。

在BIE控制台查看下发的版本是否生效，以及当前边缘核心服务模块的状态，如下图所示：

![core-version.png](https://bce.bdstatic.com/doc/undefined/core-version_95f2ade.png)

登录到树莓派，通过`docker ps`命令检查服务模块运行状态，如下图所示：

![dockerps.png](https://bce.bdstatic.com/doc/undefined/dockerps_0a6c211.png)


#### step2：使用MQTTBox订阅localhub

在[模拟场景](#模拟场景)当中提到“当检测到目标物体以后，保存抽帧图像，并同步发送一条消息到边缘hub模块”，为了监测发送到hub模块的消息，我们使用MQTTBox工具提前订阅 `video/infer/result` 这个topic的消息，如下图所示：

![localhub.png](https://bce.bdstatic.com/doc/undefined/localhub_093509b.png)


#### step3：使用摄像头检测物体

1. 手持摄像头，旋转一周，让摄像头能够扫描到办公桌上的`剪刀`、`笔记本电脑`、`书本`、`键盘`，以及坐在工位上的`人`。
2. 实时查看订阅了`hub`模块的MQTT Box的消息界面，每检测到一个目标物体，MQTT Box就能订阅到一条消息。

![scissors.png](https://bce.bdstatic.com/doc/undefined/scissors_affc33c.png)

**MQTT消息解析**

将MQTTBox订阅的消息进行Json格式化，得到如下结果：

```json
{
		"imageCaptureTime": "2019-12-05T06:32:01.095503816Z",
		"imageDiscard": false,
		"imageHight": 480,
		"imageInferenceTime": 0.680533552,
		"imageLocation": "var/db/baetyl/image/1575527521095503816.jpg",
		"imageObjects": [{
			"bottom": 0.7979179620742798,
			"class": "scissors",
			"left": 0.4228750169277191,
			"right": 0.7018369436264038,
			"score": 0.9887169599533081,
			"top": 0.29671576619148254
		}],
		"imageProcessTime": 0.691838184,
		"imageScores": {
			"book": 0.0685628280043602,
			"cell phone": 0.040239665657281876,
			"keyboard": 0.03728525713086128,
			"person": 0.037795186042785645,
			"scissors": 0.9887169599533081
		},
		"imageWidth": 640,
		"messageTimestamp": 1575527521095,
		"publishTopic": "video/infer/result"
	}
```
> 通过上述消息，可以得出如下结论：
 - 检测到物体是剪刀：`"class": "scissors"`
 - AI推断为剪刀的得分是0.988：`"scissors": 0.9887169599533081`
 - 图片已保存：`"imageLocation": "var/db/baetyl/image/1575527521095503816.jpg"`


#### step4：验证被保存的图片当中的对象

1. 登录到树莓派，查看已经保存了多张抽帧图片，如下图所示：

    ![pic.png](https://bce.bdstatic.com/doc/undefined/pic_512a1cf.png)

2. 将 `1575527521095503816.jpg` 下载到本地电脑，确认该图片当中物体是剪刀，与MQTTBox接收到消息一致，如下图所示：
    
    ![1575527521095503816.jpg](https://bce.bdstatic.com/doc/undefined/1575527521095503816_9d8c526.jpg)


## 总结

1. 通过本文，你可以设计并完成一个典型边缘视频AI解决方案，让你“0门槛”跨入AIoT的领域。
2. 虽然本文的的模拟场景比较简单，但是视频AI的整体流程已经完全覆盖，你完全可以基于这样的套路去解决真实场景的问题。


最后如果需要查看完整教程，请点击“查看原文”(https://cloud.baidu.com/doc/BIE/s/Dk3nv5gum)。

