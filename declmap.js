var declmap = {
};
var docs = {
    "overview": {
        "name": "overview",
        "title": "overview",
	"content" : "# Paracel\n\nParacel is a distributed computational framework designs for machine learning problems, graph algorithms and scientific computation in C++\n\n## Motivation\n\n * Parameters can not be hold in memory of one node\n\n * Asynchrounous learning to speed up convergency\n\n * General framework\n  - Simple communication model(compared to MapReduce)\n\n  - Fault tolerance solution(compared to MPI)\n\nOur basic idea is triggered by Jeff Dean's [talk](http://infolab.stanford.edu/infoseminar/dean.pdf) @Stanford University in 2013.\nYou can get more details in his paper: [Large Scale Distributed Deep Networks](http://static.googleusercontent.com/media/research.google.com/en//archive/large_deep_networks_nips2012.pdf)\n\n## Goals\n\n * Split both massive dataset and massive parameter space.\n\n * Solve \"the last reducer problem\" of iterative tasks.\n\n * Easy to programming, painless from serial to parallel.\n\n * Good performance and fault tolerant.\n\n## Get it\n\nYou can download the source code from http://code.dapps.douban.com/paracel:\n\n```bash\n$ git clone http://code.dapps.douban.com/paracel.git\n```\n\n## Installation\n\n * I.**Prerequisite**\n\n You must firstly install some external libraries below:\n\n  - [Boost(>=1.54)](http://www.boost.org/)\n\n  - Zeromq(>=3.2.4) and [a c++ binding](http://zeromq.org/bindings:cpp) of it\n\n  - [Msgpack-c-0.5.8](https://github.com/xunzhang/msgpack-c): a increment version\n\n  - [Eigen(>=3.0)](http://eigen.tuxfamily.org/)\n\n  - [GFlags](https://code.google.com/p/gflags/)\n\n And make sure you have:\n\n  - a impl version of MPI\n\n  - `gcc-4.`/`g++-4.7` or higher\n\n  - `autotools`\n\n *  II.**Build**\n\n```bash\n$ mkdir build; cd build;\n$ cmake -DCMAKE_BUILD_TYPE=Release ..\n$ make -j 4\n$ make install\n```\n\nIf you use gentoo, you can just use ebuild files we provide\n\nStuff @[Douban.Inc](http://www.douban.com/) can just skip **phaseI**\n\n## Get started\n\nA 20-minutes' tutorial is [here](http://xunzhang.github.io/paracel/quickstart.html)\n\n## Logo\n\nDraft version by xunzhang:\n\n```bash\n(0.5,1) -> (0, 0.5) -> (1,0.5) -> (0.5, 1) -> (0.5, 0.25) -> (0.25, 0.25)\n```\n\nPlus version by Yinzi:\n ...\n\n##Whisper \n\nIf you are using paracel, let me know.\n\nAny bugs and related problems, feel free to ping me: <wuhong@douban.com>, <xunzhangthu@gmail.com>.\n\n",
        "decl_titles": [
        ]
    }
};
