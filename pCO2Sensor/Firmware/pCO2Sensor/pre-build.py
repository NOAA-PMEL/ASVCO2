import subprocess
import os
import pathlib
import sys 
import datetime as dt



if len(sys.argv) > 1:
    path = pathlib.Path(sys.argv[1])
else:
    path = pathlib.Path('.')

cwd = os.getcwd()
os.chdir(path)

version = subprocess.run(['git', 'describe', '--tags', '--long'], capture_output=True)
version = version.stdout.decode().strip()
print(version)

read_path = path / 'version_template.h'
write_path = path / 'src/version.h'

# print(read_path)
# print(write_path)

with open(read_path, 'r') as r, open(write_path, 'w+') as w:
    for line in r:
        line = line.replace('VER_VAR', version)
        line = line.replace('VER_DATE', dt.datetime.now().strftime('%m/%d/%Y %H:%M:%S'))

        w.write(line)

os.chdir(cwd)