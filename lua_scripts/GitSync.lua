-- 实现Git工程的批量双向同步。

GIT_INSTALL_DIR = "C:\\Program Files\\Git\\"

cmd = "\"" .. GIT_INSTALL_DIR .. "bin\\git.exe" .. "\""
os.execute(cmd)
