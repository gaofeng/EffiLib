require "exec"
require "dwiap"
require "intelhex"
require "lfs"

TerminalHexFile = [[D:\Subversion\Product\DW732J_SD\Dev\Debug\Exe\DW732J_SD.hex]]
BootloaderHexFile = [[D:\��ʽ����\Bootloader\DW732J_Loader_NAND_20110718.hex]]
SevenZipExePath = [[C:\Program Files\7-Zip\7z.exe]]

PublishRootDir = [[D:\���Է���\DW732J\]]
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

--�������Է���Ŀ¼
lfs.mkdir(PublishDir)

--�ϲ�HEX�ļ�
intelhex.HexMerge(TerminalHexFile, BootloaderHexFile, PublishHexFilePath)

--����BIN�ļ�
intelhex.HexToBin(TerminalHexFile, PublishBinFilePath)
--����DAT�ļ�
dwiap.GenUpdateFile(PublishBinFilePath, PublishDatFilePath, ProductName, Version)
--ɾ��BIN��ʱ�ļ�
os.execute("del " .. PublishBinFilePath)

exec.ExecCP(SevenZipExePath .. " a " .. PublishRootDir .. PublishFileName .. ".7z " .. PublishDir)

