# Live streaming on Raspberry Pi

I try to stream camera images to smartphone. [Raspberry Pi](http://raspberrypi.org) is a my choice of system platform. 

There are mainly two kind of cameras on Rasberry Pi: [Raspberry Pi camera module](https://www.raspberrypi.org/products/camera-module-v2/) and [USB camera](https://www.logitech.com/en-us/video/webcams). I use both in my project and state different situation in the following content.

## What I need
1. [Raspberry pi 4 Model B](https://www.raspberrypi.org/products/raspberry-pi-4-model-b/) or [Respberry Zero W](https://www.raspberrypi.org/products/raspberry-pi-zero-w/)
2. Power Adapter
3. Wireless LAN
4. Raspberry Pi camera module or USB camera
5. Host computer

## Prepare Raspberry Pi
1. Prepare one SD card and [install operation system](https://projects.raspberrypi.org/en/projects/raspberry-pi-setting-up/3)
2. Connect Raspberry pi with a screen and start the operation system
3. Connect the Respberry pi into the WLAN and get the IP address
4. Host computer use SSH to login the Raspberry Pi.  
```
ssh pi@address_raspberry_pi
```
5. Connect camera module or USB camera to Raspberry Pi and enable the camera.  
```
sudo raspi-config
```
![img](http://ww1.sinaimg.cn/large/006y8mN6ly1g6yun39bklj30va0dan4c.jpg)
![img](http://ww3.sinaimg.cn/large/006y8mN6ly1g6yun71ge0j30va0dagsx.jpg)
6. Check the camera work.  
6.1 camera module.  
```
sudo nano/etc/modules
```   
Add 
```
bcm2835-v4l2
```in the last line.  
Save the file and restart the Raspberry Pi:   
```   
sudo reboot
```  
Test if the camera module works well:
```
vcgencmd get_camera
```    
And take a picture using camera module:
```
raspistill -o test.jpg
```
  
6.2 USB camera.    

input the command line:   
```
lsusb
```   
you can see the output like this:    
   ![img](http://ww3.sinaimg.cn/large/006y8mN6ly1g6yu8so3ncj30qk04qq5y.jpg)   
   or you can input the other command line:     
   ```
   ls /dev
   ```  
   you can see video0 is in the device list and the output like this:   
   ![img](http://ww1.sinaimg.cn/large/006y8mN6ly1g6yuc133jpj306s044gm6.jpg)

## Live streaming based on Nginx and RTMP
Note: only <b>camera module</b> is used in this situation

Software preparation:    
1. Nginx and RTMP.   
2. avconv and GStreamer.     

1. Install Nginx and RTMP module.   

 ```
	sudo apt-get update  
	sudo apt-get -y install nginx 
	sudo apt-get -y remove nginx
	sudo apt-get clean

	sudo rm -rf /etc/nginx/*
	sudo apt-get install -y curl build-essential libpcre3 libpcre3-dev libpcre++-dev zlib1g-dev libcurl4-openssl-dev libssl-dev
	sudo mkdir -p /var/www
	
	mkdir -p /nginx_src
	cd /nginx_src
	wget http://nginx.org/download/nginx-1.11.8.tar.gz
	wget https://github.com/arut/nginx-rtmp-module/archive/master.zip
	tar -zxvf nginx-1.11.8.tar.gz
	unzip master.zip
	cd nginx-1.11.8
	./configure --prefix=/var/www --sbin-path=/usr/sbin/nginx --conf-path=/etc/nginx/nginx.conf --pid-path=/var/run/nginx.pid --error-log-path=/var/log/nginx/error.log --http-log-path=/var/log/nginx/access.log --with-http_ssl_module --without-http_proxy_module --add-module=/home/pi/nginx_src/nginx-rtmp-module-master
	make
	sudo make install
 ```
After installation, use 
```
nginx -V
```
to check the verion of nginx.   

2. Configure Nginx

	```
	/etc/nginx/nginx .conf
	```
	Add the following content in the end.
	
	```
	rtmp {
   		server {
       		listen 1935;
       		chunk_size 4096;
       		application live {
          		live on;
          		record off;
        	}	
    	}
   }
	```
	Restart the Nginx server
	
	```
	sudo service nginx start
	```
3. Install avconv and GStreamer

	```
	sudo apt-get update
	sudo apt-get install libav-tools
	sudo apt-get install gstreamer1.0-tools
	sudo apt-get install libgstreamer1.0-0 libgstreamer1.0-0-dbg libgstreamer1.0-dev liborc-0.4-0 liborc-0.4-0-dbg liborc-0.4-dev liborc-0.4-doc gir1.2-gst-plugins-base-1.0 gir1.2-gstreamer-1.0 gstreamer1.0-alsa gstreamer1.0-doc gstreamer1.0-omx gstreamer1.0-plugins-bad gstreamer1.0-plugins-bad-dbg gstreamer1.0-plugins-bad-doc gstreamer1.0-plugins-base gstreamer1.0-plugins-base-apps gstreamer1.0-plugins-base-dbg gstreamer1.0-plugins-base-doc gstreamer1.0-plugins-good gstreamer1.0-plugins-good-dbg gstreamer1.0-plugins-good-doc gstreamer1.0-plugins-ugly gstreamer1.0-plugins-ugly-dbg gstreamer1.0-plugins-ugly-doc gstreamer1.0-pulseaudio gstreamer1.0-tools gstreamer1.0-x libgstreamer-plugins-bad1.0-0 libgstreamer-plugins-bad1.0-dev libgstreamer-plugins-base1.0-0 libgstreamer-plugins-base1.0-dev
	```
4. Push stream using avconv and GStreamer

	Because avconv using software-based encoding and decoding, the CPU cost is high.  
	```
	avconv -f video4linux2 -r 24 -i /dev/video0 -f flv rtmp://localhost:1935/live &
	```
	
	GSteamer uses hardware-based encoding and decoding, the CPU cost is low.  
	```
	gst-launch-1.0 -v v4l2src device=/dev/video0 ! 'video/x-raw, width=1024, height=768, framerate=30/1' ! queue ! videoconvert ! omxh264enc ! h264parse ! flvmux ! rtmpsink location='rtmp://address_raspberry_pi/live live=1' &
	```
5. View the stream

	1. Use RTMP supported player, like [VLC](https://www.videolan.org/vlc/):  
		Input <b>rtmp://address_raspberry_pi/live</b>  
		While VLC normally have 20s delay, you need set the cache time duration to decrease the delay
	2. Use <b>Strobe Media Playback</b> to create web page to play. The time delay is about 1s.  
		Open one web page and input <b>http://address_raspberry_pi/index.html</b>
	3. Use live streaming platform, like Douyu(斗鱼直播)


## Live streaming based on FFMpeg and FFServer


## Live streaming based on Mjpeg-streamer
Note: <b>USB camera</b> is used in this situation

 1. Install dependency library.  
 
 ```
 sudo apt-get install libjpeg62-dev libjpeg8-dev cmake
 ```
 2. Download the mjpg-streamer
 
 ```
 wget https://github.com/jacksonliam/mjpg-streamer/archive/master.zip
 unzip mjpg-streamer-master.zip

 ```
 3. Build the mjpg-streamer
 
 ```
 cd mjpg-streamer-master/mjpg-streamer-experimental
 sudo make clean all
 ```
 4. Install the mjpg-streamer
 
 ```
 make
 sudo make install
 ```
 5. Use mjpg-streamer to stream

 ```
/usr/local/bin/mjpg_streamer -i "/usr/local/lib/mjpg-streamer/input_uvc.so -n -f 30 -r 1280x720" -o "/usr/local/lib/mjpg-streamer/output_http.so -p 8080 -w /usr/local/share/mjpg-streamer/www"
 ```
 
 6. View the stream on web page.  
 Open one web page on host computer and input the url: <u><b>http://address_raspberry_pi:8080/?action=stream</u></b> and view the monitoring page.  


## reference
1. [Use MJPG-Streamer to achieve Raspberry Pi camera live streaming](https://blog.csdn.net/qq_41923622/article/details/88366185)
2. [Use Raspberry Pi as RTMP server](http://shumeipai.nxez.com/2017/11/01/build-rtmp-stream-live-server-with-raspberry-pi.html)
3. [Use Raspberry Pi and ffmpeg for live streaming](https://edmondfrank.github.io/blog/2018/02/12/shi-yong-shu-mei-pai-shi-xian-24xiao-shi-bu-jian-duan-zhi-bo/)
4. [Raspberry Pi use USB camera](https://www.jianshu.com/p/7a71e8c82734)

## Support or Contact

If any question, contact with [email](nicole2442@gmail.com) and we will help you sort it out.

