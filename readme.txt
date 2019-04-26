1、项目依赖于三菱公司的CActUtlType控件，目前已经移除。
     如需实用，需安装相关软件，并在dlg中添加相关的ActiveX控件。

2、项目依赖于opencv2.4.13版本，需要添加opencv的bin路径X:\opencv\build\x86\vc12\bin到系统环境变量Path
，同时需要修改ini文件夹下的opencvXXX.properties文件中包含的opencv路径。

3、系统可能会缺少msvcp120d.dll及msvcr120d.dll两个动态链接库文件，可以下载放置到配置的opencv的环境变量bin目录中。
