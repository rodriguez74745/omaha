import subprocess
import sys
import os

def main(argv):
  if (not "OMAHA_PROTOBUF_BIN_DIR" in os.environ):
    proto_path = "C:\\protobuf\\bin\\"
  else:
    proto_path = os.environ["OMAHA_PROTOBUF_BIN_DIR"]

  args = [
    "\\".join([proto_path, 'protoc.exe'])
  ]

  args += argv

  try:
    popen = subprocess.Popen(args, shell=True,
                             stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    out, _ = popen.communicate()
    popen.wait()
    return 0
  finally:
    pass

if __name__ == '__main__':
  sys.exit(main(sys.argv[1:]))