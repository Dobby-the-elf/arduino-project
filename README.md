### You may implement this project by using "all_att3.5.6.ino" & "bitmap1.h" only
(put them into same folder named "all_att3.5.6")
### Hardware requirement can be found in the following part (Component we uses).  
&nbsp;
# arduino-project
An arduino project for helping people with Dementia to get back home.
# 單晶片專案－防走失裝置
## 實踐功能 Features
1. 回家指路 Home position indication
2. 家屬定位和超距警吿 using wifi module to alarm familay member if patient is too far from home
3. 自我介紹 self-introduction
4. 提醒吃藥喝水 
5. 提醒離家距離 distancing

## 使用元件 Component we uses
1. Arduino mega 2560
2. 無源蜂鳴器(buzzer)：
3. [Wifi（ESP8266)](https://shopee.tw/shop/78246604/item/1384303983/rating)
4. [指南針 gy273](https://www.taiwaniot.com.tw/product/gy-273-hmc5883l-%E6%A8%A1%E7%B5%84-%E9%9B%BB%E5%AD%90%E6%8C%87%E5%8D%97%E9%87%9D%E7%BE%85%E7%9B%A4%E6%A8%A1%E7%B5%84-%E4%B8%89%E8%BB%B8%E7%A3%81%E5%A0%B4%E6%84%9F%E6%B8%AC%E5%99%A8/)
5. [TFT(ILI9225) LCD顯示器](http://www.lcdwiki.com/2.2inch_Arduino_SPI_Module_ILI9225_SKU:MAR2201)
6. [遊戲搖桿傳感器](https://www.pchomeus.com/item/show?30203662442877)
7. [PCB board of Mega(optional)](https://store.arduino.cc/usa/arduino-mega-proto-shield-rev3-pcb)
## Softwares
1.  WIFI 定位([Google Geolocation API](https://developers.google.com/maps/documentation/geolocation/overview))
2.  ioT MQTT Broker (將wifi抓到的經緯度位址上傳到sever，供家屬端存取)
3.  APP 開發
### 參考資料references
{%youtube 9CmGMYnHR-U %}
Arduino library:https://www.arduino.cc/reference/en/libraries/wifilocation/
# 瑋哥的資料
1. [URLSession 教學](https://medium.com/@jerrywang0420/urlsession-教學-swift-3-ios-part-1-a1029fc9c427)
2. [iOS x IoT](https://medium.com/彼得潘的-swift-ios-app-開發教室/ios-x-iot-1-overview-add874221174)

# iOT
###  Server 架設
使用[(Google Cloud Platform)](https://cloud.google.com/gcp/?hl=zh-tw&utm_source=google&utm_medium=cpc&utm_campaign=japac-TW-all-en-dr-bkws-all-all-trial-e-dr-1009137&utm_content=text-ad-none-none-DEV_c-CRE_450509447403-ADGP_Hybrid%20%7C%20AW%20SEM%20%7C%20BKWS%20~%20T1%20%7C%20EXA%20%7C%20General%20%7C%20M%3A1%20%7C%20TW%20%7C%20en%20%7C%20cloud%20%7C%20platform-KWID_43700020290826174-kwd-297230107829&userloc_1012818-network_g&utm_term=KW_google%20platform&gclid=CjwKCAiA8ov_BRAoEiwAOZogwRyLGRvNd8kY21VIsQSUOEentLNgvJ7R5CgnThyKAUVvjEZI98KDbRoCdNAQAvD_BwE)

1. [在 GCP 建立一個 Ubuntu (Virtual Machine)](https://medium.com/彼得潘的-swift-ios-app-開發教室/ios-x-iot-3-建立雲端-server-449a5b69ad71)
2. 使用GCP的Compute Engine 架設 Server(提供老人與家屬端的數據傳輸)


### Publish/Subscribe
#### Publish
1. 參考資料：https://swf.com.tw/?p=1089
#### Subscribe
1. Cocoa MQTT： MQTT client library
2. [Carthage](https://gist.github.com/weihanglo/97e949a9dbf92deb111999b6e42e9654)： Cocoa 開發第三方套件管理工具
3. [CocoPods](https://medium.com/彼得潘的-swift-ios-app-開發問題解答集/使用-cocoapods-管理第三方套件-6e6135b62814)：第三方軟件管理程式
### iOS App 開發
1. [Apple Developer 官方教學](https://developer.apple.com/tutorials/swiftui)
2. [Swift 語法](https://docs.swift.org/swift-book/GuidedTour/GuidedTour.html)
3. Swift 基礎：[Structs](https://medium.com/@boshilee/swift-3-基礎-structs-478f6ef3369f)
4. SwiftUI 示範影片 {%youtube hHZOAYsyd8Y %}
5. [利用 NavigationView & NavigationLink 切換頁面](https://medium.com/彼得潘的-swift-ios-app-開發問題解答集/利用-navigationview-navigationlink-切換頁面-3a062833230f)
6. [JSON 解碼](https://programmingwithswift.com/parse-json-from-file-and-url-with-swift/)


## Google map on lcd screen
https://www.youtube.com/watch?v=lQ157ftTnSs&ab_channel=ThatProject

## More details
About screen

[https://www.youtube.com/watch?v=7ER1fbDoc20&ab_channel=educ8s.tv](https://www.youtube.com/watch?v=7ER1fbDoc20&ab_channel=educ8s.tv)

Youtube CAT [https://www.youtube.com/results?search_query=2.2''+sku+mar2201+project](https://www.youtube.com/results?search_query=2.2%27%27+sku+mar2201+project)


### User Manual:

[http://www.lcdwiki.com/res/MAR2201/2.2inch_Arduino_SPI_Module_MAR2201_User_Manual_CN.pdf](http://www.lcdwiki.com/res/MAR2201/2.2inch_Arduino_SPI_Module_MAR2201_User_Manual_CN.pdf)


### PNG to BMP:

[https://convertio.co/png-bmp/](https://convertio.co/png-bmp/)

如果要輸出圖檔的話，好像會需要插記憶卡，所以如果可以的話，盡量用公式兜出來


### image to array 是一種替代方案（用比較簡單的圖案）

[https://javl.github.io/image2cpp/](https://javl.github.io/image2cpp/)


### split image

[https://pinetools.com/split-image](https://pinetools.com/split-image)


### RGB 565

![About%20Arduino%20project%202bcc5130efa5408d8032f18702602c34/_2020-11-29_3.23.13.png](About%20Arduino%20project%202bcc5130efa5408d8032f18702602c34/_2020-11-29_3.23.13.png)


### compare to RGB 888

![About%20Arduino%20project%202bcc5130efa5408d8032f18702602c34/_2020-11-29_3.23.41.png](About%20Arduino%20project%202bcc5130efa5408d8032f18702602c34/_2020-11-29_3.23.41.png)


### color converter

[https://www.rapidtables.com/convert/color/index.html](https://www.rapidtables.com/convert/color/index.html)

[http://www.rinkydinkelectronics.com/calc_rgb565.php](http://www.rinkydinkelectronics.com/calc_rgb565.php)


### SPI speed

[https://www.google.com/search?q=arduino+spi+dpeed&oq=arduino+spi+dpeed&aqs=chrome..69i57j0i13l2j0i13i30l2j0i8i13i30l2j0i5i13i30.7644j0j7&sourceid=chrome&ie=UTF-8](https://www.google.com/search?q=arduino+spi+dpeed&oq=arduino+spi+dpeed&aqs=chrome..69i57j0i13l2j0i13i30l2j0i8i13i30l2j0i5i13i30.7644j0j7&sourceid=chrome&ie=UTF-8)

[how to speed up refreshing tft display](https://forum.arduino.cc/index.php?topic=491682.0)


### SPI optimized library 

[https://github.com/TravisRo/TFT_22_ILI9225](https://github.com/TravisRo/TFT_22_ILI9225)

[https://github.com/Nkawu/TFT_22_ILI9225](https://github.com/Nkawu/TFT_22_ILI9225)

[https://github.com/ZinggJM/TFT_22_ILI9225](https://github.com/ZinggJM/TFT_22_ILI9225)

choose

[https://github.com/Nkawu/TFT_22_ILI9225](https://github.com/Nkawu/TFT_22_ILI9225)



### 困境

顯示速度不夠快（受限於library、板子、delay()）

整合後記憶體不足（library, 減肥）

腳位不足（Arduino Mega）

指北針模組異常（會有磁場干擾需校正）

176*220=38700 pixels

[https://blog.xuite.net/iamleon/blog/589243872](https://blog.xuite.net/iamleon/blog/589243872)


### 電源供應問題

[http://yhhuang1966.blogspot.com/2015/07/ic.html](http://yhhuang1966.blogspot.com/2015/07/ic.html)

[https://www.google.com/search?biw=1440&bih=821&sxsrf=ALeKk01K_SZcxmDRa-IBnFlW820UHjjdqQ%3A1609226021858&ei=JdfqX7zwM7eZr7wPw_-imAk&q=nodemcu+電源&oq=NodeMCU+電&gs_lcp=CgZwc3ktYWIQAxgBMgQIIxAnMgIIADIECAAQHjoECAAQR1CWjwFYlo8BYK6dAWgAcAJ4AIABL4gBL5IBATGYAQCgAQGqAQdnd3Mtd2l6yAEBwAEB&sclient=psy-ab](https://www.google.com/search?biw=1440&bih=821&sxsrf=ALeKk01K_SZcxmDRa-IBnFlW820UHjjdqQ%3A1609226021858&ei=JdfqX7zwM7eZr7wPw_-imAk&q=nodemcu+%E9%9B%BB%E6%BA%90&oq=NodeMCU+%E9%9B%BB&gs_lcp=CgZwc3ktYWIQAxgBMgQIIxAnMgIIADIECAAQHjoECAAQR1CWjwFYlo8BYK6dAWgAcAJ4AIABL4gBL5IBATGYAQCgAQGqAQdnd3Mtd2l6yAEBwAEB&sclient=psy-ab)

[https://www.google.com/search?sxsrf=ALeKk00638g7ikm_zMp7WLQInVORqLSZ7Q:1609225891472&q=arduino+5v不夠用&sa=X&ved=2ahUKEwiIjv_M0fLtAhXRKqYKHUk8DOcQ1QIoAHoECAUQAQ&biw=1440&bih=821](https://www.google.com/search?sxsrf=ALeKk00638g7ikm_zMp7WLQInVORqLSZ7Q:1609225891472&q=arduino+5v%E4%B8%8D%E5%A4%A0%E7%94%A8&sa=X&ved=2ahUKEwiIjv_M0fLtAhXRKqYKHUk8DOcQ1QIoAHoECAUQAQ&biw=1440&bih=821)
