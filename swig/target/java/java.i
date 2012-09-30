/** 
 @cond
 ############################################################################
 # LGPL License                                                             #
 #                                                                          #
 # This file is part of the Machine Learning Framework.                     #
 # Copyright (c) 2010-2012, Philipp Kraus, <philipp.kraus@flashpixx.de>     #
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


/** Java interface file for converting Java types and data into C++ 
 * datatypes and UBlas structurs
 * $LastChangedDate$
 **/

// ---------------------------------------------------------------------------------------------------------------------------------------------
// defines for short typemapping

%define CONSTTYPES( JNITYPE, JVTYPE, CPPTYPE )

%typemap(jni)       CPPTYPE, const CPPTYPE&      "JNITYPE"
%typemap(jtype)     CPPTYPE, const CPPTYPE&      "JVTYPE"
%typemap(jstype)    CPPTYPE, const CPPTYPE&      "JVTYPE"
%typemap(javain)    CPPTYPE, const CPPTYPE&      "$javainput"

%enddef


%define NONCONSTTYPES( JNITYPE, JVTYPE, CPPTYPE )

%typemap(jni)       CPPTYPE&       "JNITYPE*"
%typemap(jtype)     CPPTYPE&       "JVTYPE"
%typemap(jstype)    CPPTYPE&       "JVTYPE"
%typemap(javain)    CPPTYPE&       "$javainput"

%enddef
 
 
 
// ---------------------------------------------------------------------------------------------------------------------------------------------
// type converting from C++ types to Java types

CONSTTYPES( jobjectArray,        Double[],                           ublas::vector<double> )
CONSTTYPES( jobjectArray,        Double[],                           std::vector<double> )
CONSTTYPES( jobjectArray,        Double[][],                         ublas::matrix<double> )
CONSTTYPES( jobject,             java.util.ArrayList<Double[][]>,    std::vector< ublas::matrix<double> > )
CONSTTYPES( jobjectArray,        String[],                           std::vector<std::string> )
CONSTTYPES( jobjectArray,        long[],                             std::vector<std::size_t> )
CONSTTYPES( jobjectArray,        Long[],                             ublas::indirect_array<> )
CONSTTYPES( jlong,               long,                               std::size_t )
CONSTTYPES( jstring,             String,                             std::string )

NONCONSTTYPES( jobjectArray,     Double[],                           ublas::vector<double> )
NONCONSTTYPES( jobjectArray,     Double[][],                         ublas::matrix<double> )

// add the global rule, so no swigtype is created and JNI return types are passed to the Java method return
%typemap(javaout) SWIGTYPE { return $jnicall; }

// the ublas::symmetric_matrix template does not work with the define, so run it manually
%typemap(jni)       ublas::symmetric_matrix<double, ublas::upper>, const ublas::symmetric_matrix<double, ublas::upper>&      "jobjectArray"
%typemap(jtype)     ublas::symmetric_matrix<double, ublas::upper>, const ublas::symmetric_matrix<double, ublas::upper>&      "Double[][]"
%typemap(jstype)    ublas::symmetric_matrix<double, ublas::upper>, const ublas::symmetric_matrix<double, ublas::upper>&      "Double[][]"
%typemap(javain)    ublas::symmetric_matrix<double, ublas::upper>, const ublas::symmetric_matrix<double, ublas::upper>&      "$javainput"

// ---------------------------------------------------------------------------------------------------------------------------------------------
// main typemaps for "return / output types" (output = return type and call-by-reference)

%typemap(out, optimal=1, noblock=1) ublas::matrix<double>,                const ublas::matrix<double>&,
                                    ublas::vector<double>,                const ublas::vector<double>&,
                                    ublas::indirect_array<>,              const ublas::indirect_array<>&
                                    std::vector<double>,                  const std::vector<double>&
{
    $result = swig::java::getArray(jenv, $1);
}

%typemap(out, optimal=1, noblock=1) std::vector< ublas::matrix<double> >, const std::vector< ublas::matrix<double> >&
{
    $result = swig::java::getArrayList(jenv, $1);
}

%typemap(out, optimal=1, noblock=1) ublas::symmetric_matrix<double, ublas::upper>, const ublas::symmetric_matrix<double, ublas::upper>&
{
    $result = swig::java::getArray(jenv, static_cast< ublas::matrix<double> >($1));
}

%typemap(argout, optimal=1, noblock=1) const ublas::vector<double>&, const ublas::matrix<double>&,ublas::vector<double>, ublas::matrix<double>
{
}

%typemap(argout, optimal=1, noblock=1) ublas::vector<double>& (jobjectArray l_jnireturn)
{
    l_jnireturn = swig::java::getArray(jenv, *$1);
    $input = &l_jnireturn;
}

%typemap(argout, optimal=1, noblock=1) ublas::matrix<double>& (jobjectArray l_jnireturn)
{
    l_jnireturn = swig::java::getArray(jenv, *$1);
    $input = &l_jnireturn;
}

%typemap(out, optimal=1, noblock=1) std::size_t, const std::size_t&
{
    $result = $1;
}



// ---------------------------------------------------------------------------------------------------------------------------------------------
// typemaps for input paramter, that are used for type convert and declaration of additional variables. The typemaps are called before the method
// is called

%typemap(in, optimal=1, noblock=1) ublas::vector<double>& (ublas::vector<double> l_return)
{
    $1 = &l_return;
}

%typemap(in, optimal=1, noblock=1) ublas::matrix<double>& (ublas::matrix<double> l_return)
{
    $1 = &l_return;
}

%typemap(in, optimal=1, noblock=1) ublas::matrix<double>, const ublas::matrix<double>& (ublas::matrix<double> l_param)
{
    l_param = swig::java::getDoubleMatrixFrom2DArray(jenv, $input);
    $1 = &l_param;
}

%typemap(in, optimal=1, noblock=1) std::string, const std::string& (std::string l_param)
{
    l_param = swig::java::getString(jenv, $input);
    $1 = &l_param;
}

%typemap(in, optimal=1, noblock=1) std::vector<std::string>, const std::vector<std::string>& (std::vector<std::string> l_param)
{
    l_param = swig::java::getStringVectorFromArray(jenv, $input);
    $1 = &l_param;
}


%typemap(in, optimal=1, noblock=1) std::vector<std::size_t>, const std::vector<std::size_t>& (std::vector<std::size_t> l_param)
{
    l_param = swig::java::getSizetVectorFromArray(jenv, $input);
    $1 = &l_param;
}


%typemap(in, optimal=1, noblock=1) std::size_t, const std::size_t&
{
    $1 = ($1_ltype)& $input;
}



// ---------------------------------------------------------------------------------------------------------------------------------------------
// structure that is included in each cpp file
%{
#include "swig/target/java/java.hpp"
namespace swig  = machinelearning::swig;
namespace ublas = boost::numeric::ublas;
%}



// ---------------------------------------------------------------------------------------------------------------------------------------------
// Java code for loading the dynamic library
%pragma(java) jniclasscode=%{
    
    /** static call of the external library, each class that uses the native
     * interface calls must be derivated from this abstract base class, so
     * we create a own glue code of library binding
     * @bug does not work if system path(es) not set. Depended libraries eg boost_system
     * are not loaded from the machinelearning-library.
     * @todo the memory management can be worked with addShutDownHook(), so on this
     * event objects can be destroyed ( http://download.oracle.com/javase/1.4.2/docs/api/java/lang/Runtime.html#addShutdownHook%28java.lang.Thread%29 )
     **/    
    static {
        
        // first try to load the JNI library directly
        try {
            System.loadLibrary("machinelearning");
        } catch (UnsatisfiedLinkError e_link1) {
            
            // create first a temp directory for setting the native libraries
            java.io.File l_temp = new java.io.File(  System.getProperty("java.io.tmpdir") + System.getProperty("file.separator") + "machinelearning" );
            if (!l_temp.isDirectory())
                l_temp.mkdirs();
            
            String l_lib = l_temp + System.getProperty("file.separator") + System.mapLibraryName("machinelearning");
            // OSX adds *.jnilib but switch to *.dylib
            if (System.getProperty("os.name").toLowerCase().indexOf( "mac" ) >= 0)
                l_lib = l_lib.substring(0, l_lib.indexOf(".jnilib")) + ".dylib";
            
            // try to load the libraries manually from the temporary directory
            try {
                System.load(l_lib);
            } catch (UnsatisfiedLinkError e_link2) {
                
                // try to read class name
                if (Thread.currentThread().getStackTrace().length < 2)
                    throw new RuntimeException("can not determine class name");
                                
                // extract files from the Jar
                try {
                    // extract from the classname the location of the JAR (remove URL prefix jar:file: and suffix after .jar)
                    String l_jarfile = java.net.URLDecoder.decode(Class.forName(Thread.currentThread().getStackTrace()[1].getClassName()).getResource("").toString(),"UTF-8");
                    l_jarfile        = l_jarfile.substring(9, l_jarfile.lastIndexOf(".jar!")) + ".jar";
                    
                    // open the Jar file to get all Jar entries and extract the "native" subdirectory
                    java.util.jar.JarFile l_jar = new java.util.jar.JarFile( l_jarfile, true );
                    java.util.Enumeration<java.util.jar.JarEntry> l_list = l_jar.entries();
                    
                    while (l_list.hasMoreElements()) {
                        
                        String l_fileentry = l_list.nextElement().getName();

                        if ( (l_fileentry.startsWith("native/")) && (l_fileentry.charAt(l_fileentry.length()-1) != '/') ) {
                            
                            // copy stream with buffered stream because it's faster
                            java.io.InputStream l_instream            = l_jar.getInputStream(l_jar.getEntry(l_fileentry));
                            java.io.BufferedInputStream l_binstream   = new java.io.BufferedInputStream(l_instream);
                            
                            java.io.FileOutputStream l_outstream      = new java.io.FileOutputStream(l_temp.toString() + System.getProperty("file.separator") + l_fileentry.substring(7, l_fileentry.length()) );
                            java.io.BufferedOutputStream l_boutstream = new java.io.BufferedOutputStream(l_outstream);
                            
                            int l_data;
                            while ((l_data = l_binstream.read ()) != -1)
                                l_boutstream.write(l_data);
                            
                            l_binstream.close();
                            l_instream.close();
                            
                            l_boutstream.close();
                            l_outstream.close();
                            
                            l_binstream  = null;
                            l_instream   = null;
                            l_boutstream = null;
                            l_outstream  = null;
                        }
                        
                        l_fileentry = null;
                    }
                    
                    l_list    = null;
                    l_jar     = null;
                    l_jarfile = null;
                } catch(Exception e_file) { e_file.printStackTrace(); } finally {
                    System.load(l_lib);
                }
            }
            l_lib = null;
            l_temp = null;
        }
    }    
    
%}
