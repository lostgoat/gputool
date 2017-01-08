from setuptools import setup, find_packages

with open('README.md') as f:
    readme = f.read()

with open('LICENSE') as f:
    license = f.read()

setup(
    name='gputool',
    version='0.1.0',
    description='A simple tool for debugging GPU HW.',
    long_description=readme,
    author='Andres Rodriguez',
    author_email='andresx7@gmail.com',
    url='https://github.com/lostgoat/gputool',
    license=license,
    packages=find_packages(exclude=('tests', 'docs'))
)

