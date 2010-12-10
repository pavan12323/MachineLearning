/** 
 @cond
 ############################################################################
 # LGPL License                                                             #
 #                                                                          #
 # This file is part of the Machine Learning Framework.                     #
 # Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>          #
 # This program is free software: you can redistribute it and/or modify     #
 # it under the terms of the GNU Lesser General Public License as           #
 # published by the Free Software Foundation, either version 3 of the       #
 # License, or (at your option) any later version.                          #
 #                                                                          #
 # This program is distributed in the hope that it will be useful,          #
 # but WITHOUT ANY WARRANTY; without even the implied warranty of           #
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
 # GNU Lesser General Public License for more details.                      #
 #                                                                          #
 # You should have received a copy of the GNU Lesser General Public License #
 # along with this program. If not, see <http://www.gnu.org/licenses/>.     #
 ############################################################################
 @endcond
 **/

/**
 * @mainpage Machine Learning Framework
 * $Rev$
 * @author Philipp Kraus <philipp.kraus@flashpixx.de> 
 *
 * @section License
 * The framework is supported under the @subpage license
 *
 * @section requirements Requirements
 * <ul>
 * <li>ATLAS ( http://math-atlas.sourceforge.net/ )</li>
 * <li>Boost ( http://www.boost.org/ ) (iostreams with gzip and bzip2 support and thread support musst be compiled within, random device and MPI support are optional)</li>
 * <li>Boost Bindings (SVN http://svn.boost.org/svn/boost/sandbox/numeric_bindings )</li>
 * <li>GiNaC ( http://www.ginac.de/ )</li>
 * <li>LAPACK ( http://www.netlib.org/lapack/ )</li>
 * <li><i>optional Hierarchical Data Format (HDF)</i> ( http://www.hdfgroup.org/ )</li>
 * <li><i>optional Message-Passing-Interface-Support</i>
 *      <ul>
 *          <li>Open MPI ( http://www.open-mpi.org/ )</li>
 *          <li>MPICH2 ( http://www.mcs.anl.gov/research/projects/mpich2/ )</li>
 *          <li>Microsoft High Performance Computing Pack 2008 R2 SDK ( http://www.microsoft.com/downloads/en/details.aspx?FamilyID=bc671b22-f158-4a5f-828b-7a374b881172 )</li>
 *      </ul>
 * <li><i>optional GetText</i> ( http://www.gnu.org/software/gettext ) for including multilanguage support</li>
 * <li><i>optional LibXML2</i> ( http://xmlsoft.org/ ) (used by wikipedia support)</li>
 * </ul>
 *
 * @section compileroptions Compiler Option
 * toolbox compilerflags
 * <ul>
 * <li><dfn>ML_RANDOMDEVICE</dfn> for using the Boost Device Random support (required Boost Random Device Support), otherwise a Mersenne Twister is used</li>
 * <li><dfn>ML_MULTILANGUAGE</dfn> option for compiling the framework with multilanguage support (uses gettext)</li>
 * <li><dfn>ML_FILES</dfn> adds the support for HDF & CSV file reading and writing</li>
 * <li><dfn>ML_SOURCES</dfn> compiles sources in that way, that e.g. NNTP / Wikipedia data can be read directly</li>
 * <li><dfn>ML_CLUSTER</dfn> enable MPI Support for the toolbox (required Boost MPI support)</li>
 * </ul>
 * The following compiler commands should / must be set
 * <ul>
 * <li><dfn>NDEBUG</dfn> for disabling Boost and local debugging</li>
 * <li><dfn>BOOST_UBLAS_NDEBUG</dfn> for disabling Boost Ublas support</li>
 * <li><dfn>BOOST_NUMERIC_BINDINGS_BLAS_CBLAS</dfn> add CBLAS / LAPACK support for the Boost Bindings</li>
 * </ul>
 *
 * @section defs definitions / specifications
 * <ul>
 * <li>data points should be matrix data and the matrix is row-orientated, so for K data points with every point dimension P, we have a K x P matrix (prototype matrices are equal)</li>
 * <li>all compiler flags start with "ML_"</li>
 * <li>all structures are in the namespace "machinelearning"</li>
 * <li>all messages should get the structure <dfn>_("<message>")</dfn>, because the underline prefix is the support for different languages</li>
 * </ul>
 *
 * @section ex advanced documentation
 * <ul>
 * <li>@subpage classifier</li>
 * <li>@subpage clustering</li>
 * <li>@subpage sources</li>
 * <li>@subpage distances</li>
 * <li>@subpage dimreduce</li>
 * <li>@subpage files</li>
 * <li>@subpage logger</li>
 * <li>@subpage lang</li>
 * </ul>
 *
 *
 *
 * @page license GNU Lesser General Public License
 * @verbinclude license.txt
 *
 *
 *
 * @page classifier classifier
 * @section lazy lazy learner
 * @code
    ublas::matrix<double> data = / fill data (row orientated) /;
    std::vector<std::string> labels = / fill label for each row /;
 
    // create distance object
    distances::euclid<double> d;
    // neighbourhood k-nearest-neighbour object with 4 neighbours
    neighbourhood::knn<double> k(d, 4);
    // create lazy learner object with inverse-distance-weightening 
    classifier::lazylearner<double, std::string> ll(k, classifier::lazylearner<double, std::string>::inversedistance);
 
    // insert data
    ll.setDatabase( data,  labels );
 
 * @endcode
 *
 *
 *
 * @page clustering clustering
 * The clustering classes are templates classes, there are two main structures <i>supervised</i> and <i>nonsupervised</i>. The namespaces are named in the same way machinelearning::clustering::nonsupervised or 
 * machinelearning::clustering::supervised. All classes are subclasses of their supervised / unsupervised base class. Especially the supervised classes have two template parameters, first for the data and second
 * for their labels, the nonsupervised uses only one template parameter. The examples show the different using.
 *
 * @section kmeans k-Means
 * @code
    ublas::matrix<double> data = / fill data (row orientated) /;

    // create euclidian distance object
    distances::euclid<double> d;
    // create kmeans with 11 prototypes and column dimension of data
    clustering::nonsupervised::kmeans<double> kmeans(d, 11, data.size2());
    // enabled logging
    kmeans.setLogging(true);
    // clustering with 15 iterations
    kmeans.train(data, 15);

    // if logging is enabled
    if (kmeans.getLogging()) {
        
        // get prototypes after every iteration 
        std::vector< ublas::matrix<double> > p = kmeans.getLoggedPrototypes();

        for(std::size_t i=0; i < p.size(); ++i)
            std::cout << p[i] << std::endl;
    }
 
    // get row index for the prototype of every data point
    ublas::matrix<double> unkown = / create matrix with unkown data /;
    ublas::indirect_array< std::vector<std::size_t> > protoidx = kmeans.use(unkown);
 * @endcode
 *
 * @section ng neural gas
 * @code
    ublas::matrix<double> data = / fill data (row orientated) /;
 
    // create distance object
    distances::euclid<double> d;
    // create neural gas object with 11 prototypes
    clustering::nonsupervised::neuralgas<double> ng(d, 11, data.size2());
    // enabled logging
    ng.setLogging(true);
    // clustering with 15 iterations
    ng.train(data, 15);
 
    // if logging is enabled
    if (ng.getLogging()) {
 
        // get prototypes after every iteration 
        std::vector< ublas::matrix<double> > p = ng.getLoggedPrototypes();
 
        for(std::size_t i=0; i < p.size(); ++i)
            std::cout << p[i] << std::endl;
    }
 
    // get row index for the prototype of every data point
    ublas::matrix<double> unkown = / create matrix with unkown data /;
    ublas::indirect_array< std::vector<std::size_t> > protoidx = ng.use(unkown);
 * @endcode
 *
 * @section rlvq relevance learning vector quantization (RLVQ)
 * @code
    ublas::matrix<double> data = / fill data (row orientated) /;
    std::vector<std::string> labels = / fill label for each row /;
    std::vector<std::string> prototypelabel = / fill label for each prototype /;
 
    // create distance object
    distances::euclid<double> d;
    // create rlvq object (the size of label vector is the number of prototypes)
    clustering::supervised::rlvq<double, std::string> rlvq(d, prototypelabel, data.size2())
 
    // train prototypes in 15 iterations
    rlvq.train(data, labels, 15);
 * @endcode
 *
 * @section spcl spectral clustering
 * @code
    ublas::matrix<double> data = / similarity matrix /;
 
    // create spectral clustering object with 3 cluster
    clustering::nonsupervised::spectralclustering<double> spectral(3, data.size1());
    // run clustering with 15 iteration
    spectral.train(data, 15);
 * @endcode
 *
 *
 *
 * @page dimreduce dimension reducing
 * The dimension reducing classes are in the namespace machinelearning::dimensionreduce::nonsupervised and machinelearning::dimensionreduce::supervised can be used
 * for dimension reduction. Supervised uses two template parameter first one for datatype, second one for labeling. The nonsupervised is used only one parameter
 * (for datatype). The examples show the different option
 *
 * @section pca principle component analysis (PCA)
 * @code
    ublas::matrix<double> data = / fill data (row orientated) /;
 
    // create a pca object for reduce data to 2 dimensions
    dimensionreduce::nonsupervised::pca<double> pca(2);
    
    // create reduce data matrix
    ublas::matrix<double> reduce = pca.map(data);
 * @endcode
 *
 * @section lda linear discriminant analysis (LDA)
 * @code
    ublas::matrix<double> data      = / fill data (row orientated) /;
    std::vector<std::string> labels = / fill label for each row /;
 
    // create lda object for reduce data to 2 dimensions
    dimensionreduce::supervised::lda<double, std::string> lda(2);
 
    // create reduce data
    ublas::matrix<double> reduce = lda.map(data, labels);
 * @endcode
 *
 * @section mds multidimensional scaling (MDS)
 * @code
    ublas::matrix<double> data = / fill similarity data /;
 
    // create a mds object for reduce data to 2 dimensions (the optional second parameter adds the projection type
    // The algorithms are: metric, sammon, hit (High-Throughput)
    dimensionreduce::nonsupervised::mds<double> mds(2);
 
    // the methods are only used by iterative algorithms like sammon (default values are set within the constructor)
    mds.setIteration( 120 );
    mds.setStep( 40 );
  
    // create reduce data
    ublas::matrix<double> reduce = mds.map(data);
 * @endcode
 *
 * @section lle local linear embedding (LLE)
 * @code
 * @endcode
 *
 *
 *
 * @page distances distances
 * The namespace machinelearning::distances holds all types of distances. Every distance function is a subclass of <i>distance</i> and calculates distances values for vector- and matrixdata.
 * The class must be implementated as a template class and must hold some special functions for using the distance operation. In the namespace is also the ncd-class that creates a
 * symmetric/asymmetric dissimilarity matrix of string- or filedata with the <i>normalized compression distance</i>, that based on an approximation of the the Kolmogorov complexity. The
 * example show how to use these classes.
 *
 * @section ncd normalize compression distance (NCD)
 * @code
    std::vector< std::string > val;
    val.push_back( / add file path or string data / );
     
    distances::ncd ncd1( distances::ncd::gzip );
    ncd1.setCompressionLevel( ncd::bestspeed );
    std::cout << "read data as file and use gzip" << std::endl; 
    std::cout << "unsymmetric: " << ncd1.unsymmetric<double>(val, true) << std::endl;
    std::cout << "symmetric: " << ncd1.symmetric<double>(val, true) << std::endl;
 
    distances::ncd ncd2( distances::ncd::bzip2 );
    ncd2.setCompressionLevel( ncd::bestcompression );
    std::cout << "read data as string and use bzip2" << std::endl; 
    std::cout << "unsymmetric: " << ncd1.unsymmetric<double>(val) << std::endl;
    std::cout << "symmetric: " << ncd1.symmetric<double>(val) << std::endl;
 * @endcode
 *
 *
 *
 * @page sources data sources
 * The toolbox implements some structures for geting datasets. The compile option <dfn>ML_SOURCES</dfn> must be set for compiling the namespace
 * machinelearning::tools::sources .
 * @section nntp newsgroups
 * The class allows to browse newsgroups in a naitve way for extraction the message content. The class throws different exception if there
 * are connection or stream errors during sending or receiving data (see exceptions within the class).
 * @code
    tools::sources::nntp news("<news server>");
    
    // read group list
    std::map<std::string, std::size_t> groups = news.getGroupList();
    for (std::map<std::string, std::size_t>::iterator it = groups.begin(); it != groups.end(); ++it)
        std::cout << it->first << "     (" << it->second << ")" << std::endl;
    std::cout << "\n===================================================================================" << std::endl;
 
    // sets which part of a messages is read
    news.setContent( tl::sources::nntp::full );
 
    // sets the newsgroup for browsing 
    news.setGroup("<newsgroup>");
 
    // browse each article in the group
    for(tools::sources::nntp::iterator it=news.begin(); it != news.end(); ++it) {
        
        // splits header and body of the message (should be only used with full articles)
        std::string header, body;
        tools::sources::nntp::separateHeaderBody( *it, header, body );
 
        std::cout << header << "\n-----------------------------------------------------------------------------------\n";
        std::cout << body   << "\n===================================================================================" << std::endl;
    }
 * @endcode
 *
 * @section wiki wikipedia
 * The wikipedia class can be used for read the article data of wikipedia articles in different languages
 * @code
    tools::sources::wikipedia wiki; // optional parameter for language (see class)
 
    // reads a random article
    wiki.getRandomArticle();
    // or read a article with name
    wiki.getArticle("<article name>");
 
    // create a vector for article labels or acronyms
    std::vector<std::string> tags;
 
    // check if result is an article
    if (wiki.isArticle()) {
        std::cout << wiki.getArticleContent() << "\n===================================================================================" << std::endl;
        tags = wiki.getArticleLabel();
    } else
        tags = wiki.getArticleAcronym();
 
    // write arcronym / label data
    for(std::size_t i=0; i < tags.size(); ++i)
        std::cout << tags[i] << std::endl;
 * @endcode
 *
 * @section cloud cloud
 * The cloud class creates a multimodal n-dimensional data set with normal distribution. The n-dimensional cube is sampled in equidistant
 * steps and on the cross-points a normal distribution is created.
 * @code
    // creates a plane with clouds (parameter is the number of dimensions)
    tools::sources::cloud<double> cloud(2);
 
    // set (random) number of points within the cluster
    cloud.setPoints( 100, 1000 );
 
    // sets the variance for each cluster (the value is random generated, see within the class for disabling random values)
    cloud.setVariance(0.2, 0.8);
 
    // sets the ranges and sampling point for each dimension (dimension, start value, end value, number of samples)
    cloud.setRange(0, 0, 100, 20);
    cloud.setRange(1, 0, 100, 20);
 
    // the method generate supports different parametes, see the parameters within the class
    std::cout << cloud.generate() << std::endl; 
 * @endcode
 * 
 *
 *
 * @page logger logger
 * Within the toolbox is a logger class which implements a thread-safe and optional MPI logger. The logger create a singletone object that create
 * a file access for writing messages. The MPI component sends all messages with non-blocking communication to the CPU 0. See in the logger class
 * for log states, which must be used for writing the messages.
 * @section normal normal use
 * @code
    // sets the log level for writing messages 
    tools::logger::getInstance()->setLevel( tools::logger::info );
 
    // creates a message
    tools::logger::getInstance()->write( tools::logger::warn, "test message" );
 
    // shows the filename in which the messages are write down
    std::cout << tools::logger::getInstance()->getFilename() << std::endl;
 * @endcode
 *
 * @section mpi mpi use
 * <strong>The MPI library must be compiled with thread-support and must be initialized manually</strong>
 * @code
    MPI::Init_thread( argc, argv, MPI_THREAD_SERIALIZED )
    boost::mpi::communicator l_mpi;
 
    // create the listener 
    tools::logger::getInstance()->startListener( l_mpi );
 
 
    // sets the log level for writing messages (it is seperated for each CPU)
    tools::logger::getInstance()->setLevel( tools::logger::info );
 
    // creates a message in the local log
    tools::logger::getInstance()->write( tools::logger::warn, "test message" );
 
    // creates a message with MPI use
    tools::logger::getInstance()->write( l_mpi, tools::logger::warn, "test message with MPI" );
 
    // close the listener
    tools::logger::getInstance()->shutdownListener( l_mpi );
 
    // shows the filename of each CPU (only CPU 0 collectes all messages with MPI support)
    std::cout << tools::logger::getInstance()->getFilename() << std::endl;
    MPI::Finalize()
 * @endcode
 *
 *
 *
 * @page files file support
 * The toolbox supports different file formats. The compile option <dfn>ML_FILES</dfn> must be set for
 * using the support and all components are within the namespace machinelearning::tools::files
 * @section csv comma separated values (csv)
 * @code
    tools::sources::csv csv;
 
    // read matrix data (optional second parameter is a string with separator characters)
    // the file content must be followed this order:
    // <number or rows> <number of columns>
    // 1. row: 1. column, 2.column ....
    // ....
    boost::numeric::ublas::matrix<double> matrix = csv.readBlasMatrix<double>("<filename>");
 
    // read vector data
    // the file content must be followd this order:
    // <number of elements>
    // 1. element
    // ....
    boost::numeric::ublas::vector<double> vector = csv.readBlasVector<double>("<filename>");
 
    // read any vector data
    // the file content must be followd this order:
    // 1. element
    // 2. element
    // ....
    std::vector<T> vec = csv::readVector<T>("<filename>");
 
 
    // write data
    csv.write<double>("<filename>", vector);
    // third parameter set the separater character
    csv.write<double>(""<filename>", matrix);
 * @endcode
 *
 * @section hdf hierarchical data format (hdf)
 * HDF files need on reading and writing own data formats that description can found on http://www.hdfgroup.org/HDF5/doc/cpplus_RM/classH5_1_1PredType.html 
 * @code
    // only read access
    tools::files::hdf source("<path to hdf file>");
 
    // read matrix data
    boost::numeric::ublas::matrix<double> matrix = source.readMatrix<double>("<path to dataset>", H5::PredType::NATIVE_DOUBLE);
    // read vector data
    boost::numeric::ublas::vector<unsigned int> vector = source.readVector<unsigned int>("<path to dataset>", H5::PredType::NATIVE_UINT);
    // read string
    std::string str = source.readString("<path to data>");
 
    // create a new & empty hdf file (read / write access)
    tools::files::hdf target("path to hdf file", true);
    target.write<double>( "<path for dataset>",  matrix, H5::PredType::NATIVE_DOUBLE );
    target.write<double>( "<path for dataset>",  vector, H5::PredType::NATIVE_DOUBLE );
 
    // hdf file will be closed and flushed if variable lost the scope
 * @endcode
 *
 *
 *
 @page lang multilanguage support
 * The toolbox uses <dfn>gettext</dfn> to translate the messages. The source code must include the language header file and all messages,
 * which should be supported multilanguage must be written in this way <dfn>_("messages")</dfn>. The mesages will be collcted with the
 * gettext tools and safed under <dfn>tools/language/#language eg. de_DE.UTF-8#/LC_MESSAGES/#language eg de_DE#.po</dfn>. The compiler flag
 * <dfn>ML_MULTILANGUAGE</dfn> must be set for using the support. The message files can be edited with Poedit ( http://www.poedit.net/ ).
 * For using the language support, a call for setting the correct languages must be written wihin the main program:
 * @code
    tools::language::bind(<name of the language file, should be "ml">, <path to the language directory, shoudl be "./tools/language/">);
 * @endcode
**/

#ifndef MACHINELEARNING_H
#define MACHINELEARNING_H


/** main namespace for the framework
 * $LastChangedDate: 2010-12-03 01:19:39 +0100 (Fr, 03 Dez 2010) $
 **/
namespace machinelearning {};


#include "exception/exception.h"
#include "dimensionreduce/dimensionreduce.h"
#include "classifier/classifier.h"
#include "clustering/clustering.h"
#include "neighborhood/neighborhood.h"
#include "distances/distances.h"
#include "tools/tools.h"
#include "functionoptimization/functionoptimization.h"

#endif