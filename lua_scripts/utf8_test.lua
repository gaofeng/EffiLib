converter = require "EncodeConvert"

name = "高峰"

function Str2Hex(str,spacer)
	return (
		string.gsub(str,"(.)",
			function (c)
				return string.format("%02X%s",string.byte(c), spacer or "")
			end)
	)
end

utf8_name = "世界，你好"

gbk_name = converter.u82a(utf8_name)

print(utf8_name)
print(Str2Hex(utf8_name, " "))

print(gbk_name)
print(Str2Hex(gbk_name, " "))

