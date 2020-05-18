WORKDIR=${1:-~/Programming/.cmake_builds/sb-mapreduce/debug}

# https://stackoverflow.com/questions/786376/how-do-i-run-a-program-with-a-different-working-directory-from-current-from-lin
(cd "${WORKDIR}" && exec exe/dev-demo)
