1、此为工程模版，方便日后建立新的工程文件，可删除Project文件夹下的内容重新建立新的工程保存到此处。

2、新建工程后点击“魔术棒”，在C/C++栏下的Define下添加下面内容：USE_STDPERIPH_DRIVER,STM32F10X_HD
	include patch下修改头文件目录，添加...\Libraries\FWlib\inc文件夹、...\Libraries\CMSIS文件夹和...\User文件夹
	Debug和Utilities栏下的Debug工具选为STLink，再点击后面setting按钮选择方式为SW模式(如果使用JTAG，则选为JTAG)，
	再添加 Flash_Download芯片为STM32F10x High-density  512K的芯片（芯片根据实际使用选择）
	
3、	修改stm32f10x_conf.h 文件，根据使用外设情况去掉相应外设的include注释屏蔽。

4、
