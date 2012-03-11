-- 实现Git工程的批量双向同步。

require "exec"

DirList = {}
DirList[1] = {}
DirList[1].Name = "Test"
DirList[1].WorkPath = [[F:\gittest]]

DirList[2] = {}
DirList[2].Name = "nikki"
DirList[2].WorkPath = [[D:\Documents\nikki]]
DirList[2].RemoteURL = [[I:\nikki.git]]

function RunGit(dir, arg)
	print(exec.ExecCP("git --git-dir=" .. dir .. [[\.git]] .. " " .. "--work-tree=" .. dir .. " " .. arg))
end

function GitStatus(index)
	RunGit(DirList[index].WorkPath, "status")
end

--注释编码不是UTF8，汉字会乱码
function GitCommit(index)
	print("Please input comment:")
	local msg = io.read()
	RunGit(DirList[index].WorkPath, "commit --all --message=" .. msg)
end

function GitPull(index)
	RunGit(DirList[index].WorkPath, "pull -v --progress " .. DirList[2].RemoteURL)
end

GitStatus(2)
GitPull(2)

