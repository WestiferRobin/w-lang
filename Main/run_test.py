import sys, os
      
pathname = os.path.dirname(sys.argv[0])   
test_path = f'{os.path.abspath(pathname)}/test'
test_files = os.listdir(test_path)

for test_file in test_files:
    print(f'RUNNING TEST FOR: {test_file}')
    flag = os.system(f'./wlang test/{test_file}')
    print(f'FINISHED TEST WITH {flag} CODE\n')