#! /usr/bin/env bash

function log_info()
{
  log_ "info" "$@"
}

function log_error()
{
  log_ "error" "$@"
}

function log_()
{
  local type=$1
  local message=$2
  local date_str=; date_str=$(date +'%Y-%m-%d %H:%M:%S')
  echo "[${type}] [${date_str}] ${message}"
}

function main()
{
  log_info "Stage #1 Compiling..."
  if ! ( cd ../src && make ) ; then
    log_error "Failed to compile."
    return 1
  fi

  cd ../test && mkdir tests_n_ans
  log_info "Stage #2 Test generating..."
  if ! python3 generator.py . ; then
    log_error "Failed to generate tests."
    return 1
  fi

  log_info "Stage #3 Checking..."
  tests=tests_n_ans
  for test_file in $( cd tests_n_ans && ls *.txt ) ; do
    echo ">>Checking "${test_file}"..."
    ../src/lab4 < ${tests}/${test_file}
    ret=$?
    if [ ${ret} -eq 5 ]; then
      log_info "Division by zero"
    elif [ ${ret} -eq 1 ]; then
      log_error "Failed to run test"  
      return 1
    fi
  done

  cd ../src && make clean && cd ../test
}

main $@
