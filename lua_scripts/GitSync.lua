-- 实现Git工程的批量双向同步。

require "exec"
GIT_INSTALL_DIR = [[C:\Program Files\Git\]]

rungit = GIT_INSTALL_DIR .. [[bin\git.exe]] 

function RunGit(arg)
	if arg == nil then
		arg = ""
	end
	print(exec.ExecCP(rungit .. " " .. arg))
end

function GitStatus(arg)
	if arg == nil then
		arg = ""
	end
	RunGit(arg .. " status " .. arg)
end

GitStatus()

