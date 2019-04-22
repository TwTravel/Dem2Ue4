import sys
import subprocess
from util.arg_parser import ArgParser
from util.logger import Logger

# _cmd = '--arg_file args/train_humanoid3d_walk_args_02.txt --num_workers 10'
_cmd = '--arg_file args/train_humanoid3d_ext_run_args.txt --num_workers 10'

def main():
    # Command line arguments
    args = _cmd.split() #sys.argv[1:]
    arg_parser = ArgParser()
    arg_parser.load_args(args)

    num_workers = arg_parser.parse_int('num_workers', 1)
    assert(num_workers > 0)

    Logger.print('Running with {:d} workers'.format(num_workers))
    cmd = 'mpiexec -n {:d} python DeepMimic_Optimizer.py '.format(num_workers)
    cmd += ' '.join(args)
    Logger.print('cmd: ' + cmd)
    subprocess.call(cmd, shell=True)
    return

if __name__ == '__main__':
    main()
