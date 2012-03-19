# vim: set filetype=python:

from glob import glob

env = Environment()

tests = []
for src in glob('t/*.c') + glob('t/*.cc'):
    test = env.Program(src.rstrip('.cc').rstrip('.c') + '.t', [
        src
    ])
    tests.append(test)

env.Command('test', tests, 'prove --source Executable t/*.t')
