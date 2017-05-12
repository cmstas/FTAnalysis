### So you want to outsmart those hackers? Use machine learning!


#### Making a BDT is like making a wall...
<img src="http://i.imgur.com/iZzFTzp.png" width="400">

#### Install xgboost
```bash
git clone --recursive https://github.com/dmlc/xgboost
cd xgboost/
make -j10
cd python-package/
# numpy and scipy already come with CMSSW, so kick 'em out!
# we also call this "building the wall"
sed -i "s/'numpy',/# the wall/" setup.py
sed -i "s/'scipy',/# the wall/" setup.py
# path magic
export PYTHONPATH=$(pwd)/lib/python2.7/site-packages/:$PYTHONPATH
mkdir -p $(pwd)/lib/python2.7/site-packages/
python setup.py install --prefix $(pwd)
```

#### Install root_numpy
```bash
curl -O "https://pypi.python.org/packages/12/d6/8326df573a8e5e6d293c5363e88c41f0eed4deb6573d9e8b72ba38302fa7/root_numpy-4.7.2.tar.gz"
tar xvzf  root_numpy-4.7.2.tar.gz
cd root_numpy-4.7.2
# path magic
export PYTHONPATH=$(pwd)/lib/python2.7/site-packages/:$PYTHONPATH
mkdir -p $(pwd)/lib/python2.7/site-packages/
LDFLAGS="-L/data/userdata/pgsql/pgAdmin\ 4/venv/lib/python2.7/config/" python setup.py install --prefix $(pwd)
```
