require "exec"
require "dwiap"
require "intelhex"
require "lfs"

TerminalHexFile = [[D:\Subversion\Product\DW732J_SD\Dev\Debug\Exe\DW732J_SD.hex]]
BootloaderHexFile = [[D:\正式发布\Bootloader\DW732J_Loader_NAND_20110718.hex]]
SevenZipExePath = [[C:\Program Files\7-Zip\7z.exe]]

PublishRootDir = [[D:\测试发布\DW732J\]]
Version = [[V1.28]]
Product = [[DW732J]]
ProductName = [[DW732J_SD]]

---------------------------------------------------------------------------------------------------
PublishDir = PublishRootDir .. Product .. "_" .. Version .. "\\"

CurrentDate = os.date("*t")
DateStr = string.format("%02d%02d%02d", CurrentDate.year % 100, CurrentDate.month, CurrentDate.day)
PublishFileName = ProductName .. "_" .. Version .. "_" .. DateStr
PublishHexFilePath = PublishDir .. PublishFileName .. ".hex"
PublishBinFilePath = PublishDir .. PublishFileName .. ".bin"
PublishDatFilePath = PublishDir .. PublishFileName .. ".dat"

--创建测试发布目录
lfs.mkdir(PublishDir)

--合并HEX文件
intelhex.HexMerge(TerminalHexFile, BootloaderHexFile, PublishHexFilePath)

--生成BIN文件
intelhex.HexToBin(TerminalHexFile, PublishBinFilePath)
--生成DAT文件
dwiap.GenUpdateFile(PublishBinFilePath, PublishDatFilePath, ProductName, Version)
--删除BIN临时文件
os.execute("del " .. PublishBinFilePath)

exec.ExecCP(SevenZipExePath .. " a " .. PublishRootDir .. PublishFileName .. ".7z " .. PublishDir)

