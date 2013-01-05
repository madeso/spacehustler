import subprocess
import os

cmd = "astyle --style=ansi -t3 -N -j -k1 -z1 -n -r -U -p -q -w -Y -L -S "
cwd = os.getcwd() + "\\"

def astyle(dir):
	subprocess.call(cmd + "*.h", cwd=cwd+dir)
	subprocess.call(cmd + "*.cpp", cwd=cwd+dir)
	subprocess.call(cmd + "*.hpp", cwd=cwd+dir)
	subprocess.call(cmd + "*.inl", cwd=cwd+dir)

print(cwd)

astyle("src")
