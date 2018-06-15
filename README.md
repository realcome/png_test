# PNG测试项目

* 提交PNG测试项目
* 依赖于libpng和zlib
* 增加此项目的目的主要用户png测试
* 因为我们的浏览器UI在某些机器上会出现红色色块
* 发现Chromium的代码在进行UI PNG解码时会出现随机性失败的情况
* 经过调查发现是某些CPU SSSE3指令集在进行128位数据计算时会出现随机性的错误。
* 写此项目就是为了测试在你的机器上是否会出现此SSSE3的BUG。
* 在对话框的关于中有相关的CPU指令集检测

### 用于项目生成的python脚本:
```python
#coding=utf-8

import os
import subprocess

if __name__ == "__main__":

	os.environ["DEPOT_TOOLS_WIN_TOOLCHAIN"]= "0"
	os.chdir("src")
	subprocess.call('gn gen out\\debug --args="target_cpu=\\"x86\\" is_component_build=false is_debug=false " --ide=vs2017 --sln=test_png --winsdk=\"10.0.15063.0\"',shell=True)
```


### 此项目使用gn_build
  此项目使用[gn_build](https://github.com/realcome/gn_build)项目进行构建
  
### 代码提取
建立```.gclient```文件:
```
solutions = [{
  'managed': False,
  'name': 'src',
  'url': 'https://github.com/realcome/png_test.git',
  'deps_file': 'DEPS',
}]
```
然后运行 ```gclient sync```
