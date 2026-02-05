#!/usr/bin/env python3
"""Serial monitor helper.

Runs one of: python -m serial.tools.miniterm, picocom, or screen with sane defaults.
Optionally runs `stty` to put the port into raw mode and disable flow control.

Usage:
  Tools/monitor.py [/dev/ttyUSB0] [115200] [--stty-raw] [--tool miniterm]
"""

import argparse
import shlex
import subprocess
import sys


def run_stty_raw(port, baud):
    cmd = ['stty', '-F', port, 'raw', str(baud), 'cs8', '-cstopb', '-parenb', '-ixon', '-ixoff', '-crtscts']
    try:
        subprocess.run(cmd, check=True)
    except FileNotFoundError:
        print('stty not found; skipping stty configuration')
    except subprocess.CalledProcessError:
        print('stty failed; continuing anyway')


def main():
    p = argparse.ArgumentParser(description='Serial monitor helper')
    p.add_argument('port', nargs='?', default='/dev/ttyUSB0', help='Serial device')
    p.add_argument('baud', nargs='?', default='115200', help='Baud rate')
    p.add_argument('--stty-raw', action='store_true', help='Run `stty` to set raw mode and disable flow control')
    p.add_argument('--tool', choices=['miniterm', 'picocom', 'screen'], default='picocom', help='Tool to use')
    args = p.parse_args()

    if args.stty_raw:
        run_stty_raw(args.port, args.baud)

    if args.tool == 'miniterm':
        print('Using miniterm; make sure to exit with Ctrl-t + q')
        cmd = [sys.executable, '-m', 'serial.tools.miniterm', args.port, str(args.baud)]
    elif args.tool == 'picocom':
        print('Using picocom; make sure to exit with Ctrl-a + Ctrl-x')
        cmd = ['picocom', '-b', str(args.baud), args.port]
    else:
        cmd = ['screen', args.port, str(args.baud)]

    print('Running:', ' '.join(shlex.quote(x) for x in cmd))
    try:
        subprocess.run(cmd)
    except FileNotFoundError:
        print('Requested tool not found. Install it or use --tool=miniterm with pyserial installed.')


if __name__ == '__main__':
    main()
