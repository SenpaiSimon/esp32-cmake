#!/usr/bin/env python3
"""Build and run an esptool.py write_flash command from
`build/flasher_args.json`.

Usage:
  Tools/flash.py [PORT] [BAUD]

Example:
  Tools/flash.py /dev/ttyUSB0 460800
"""

import argparse
import json
import os
import shlex
import subprocess
import sys


FLASHER_ARGS = os.path.join('build', 'flasher_args.json')


def load_flasher_args(path=FLASHER_ARGS):
    with open(path, 'r') as f:
        return json.load(f)


def build_esptool_cmd(j, port, baud):
    chip = j.get('extra_esptool_args', {}).get('chip', 'esp32')
    before = j.get('extra_esptool_args', {}).get('before')
    after = j.get('extra_esptool_args', {}).get('after')

    cmd = ['esptool.py', '--chip', chip, '--port', port, '--baud', str(baud)]
    if before:
        cmd += ['--before', before]
    if after:
        cmd += ['--after', after]

    cmd += ['write_flash', '-z']
    cmd += j.get('write_flash_args', [])

    # `flash_files` maps offsets to files (paths are relative to build/)
    for off, fn in sorted(j.get('flash_files', {}).items()):
        path = os.path.join('build', fn)
        cmd += [off, path]

    return cmd


def main(argv=None):
    argv = argv if argv is not None else sys.argv[1:]
    parser = argparse.ArgumentParser(description='Flash using build/flasher_args.json')
    parser.add_argument('port', nargs='?', default='/dev/ttyUSB0', help='Serial port')
    parser.add_argument('baud', nargs='?', default=460800, type=int, help='Baud rate')
    parser.add_argument('--dry-run', action='store_true', help="Print command but don't run it")
    args = parser.parse_args(argv)

    if not os.path.exists(FLASHER_ARGS):
        print(f"Error: {FLASHER_ARGS} not found. Run your build step first.")
        return 2

    try:
        j = load_flasher_args(FLASHER_ARGS)
    except Exception as e:
        print('Failed to read flasher args:', e)
        return 3

    cmd = build_esptool_cmd(j, args.port, args.baud)
    print('Running:', ' '.join(shlex.quote(x) for x in cmd))

    if args.dry_run:
        return 0

    try:
        subprocess.run(cmd, check=True)
    except subprocess.CalledProcessError as e:
        print('esptool failed with return code', e.returncode)
        return e.returncode

    return 0


if __name__ == '__main__':
    sys.exit(main())