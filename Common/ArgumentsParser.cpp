// ================================================================================ //
// Copyright (c) 2011 Arturo Garcia, Francisco Avila, Sergio Murguia and Leo Reyes	//
//																					//
// Permission is hereby granted, free of charge, to any person obtaining a copy of	//
// this software and associated documentation files (the "Software"), to deal in	//
// the Software without restriction, including without limitation the rights to		//
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies	//
// of the Software, and to permit persons to whom the Software is furnished to do	//
// so, subject to the following conditions:											//
//																					//
// The above copyright notice and this permission notice shall be included in all	//
// copies or substantial portions of the Software.									//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR		//
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			//
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE		//
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER			//
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,	//
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE	//
// SOFTWARE.																		//
// ================================================================================ //

#include <ArgumentsParser.h>
ArgumentsParser::ArgumentsParser() :
	/* These are the default values for the Ray Tracer */
	m_iNumThreads(1), 
	m_sAccelerationStruct("bvh"), 
	m_bIsReflective(true), 
	m_bIsMultiplicative(true),
	m_iNumReflections(0), 
	m_fSpeed(0.1), 
	m_iTextureWidth(1024), 
	m_iTextureHeight(1024), 
	m_iScreenMultiplier(1),
	m_sModel("teapot.txt"), 
	m_iMaxPrimsNode(1), 
	m_sBVHSplit("sah"), 
	//m_pError(NULL),
	m_iGroupSizeX(0),
	m_iGroupSizeY(0), 
	m_iGroupSizeZ(0), 
	m_iLBVHDepth(21), 
	m_iIterations(1)
{
	LoadConfigurationFromFile("conf.ini");
	ParseData();
	ShowConfiguration(0);
}


ArgumentsParser::~ArgumentsParser()
{
 // if(m_pKeyFile)
	//g_key_file_free(m_pKeyFile);
  if(m_pKeyFile != "")
	m_pConfig->~Config();
}

int ArgumentsParser::ParseData()
{
	m_iNumThreads =			m_pConfig->pInt("num_threads");
	m_bIsReflective =		m_pConfig->pBool("is_reflective");
	m_bIsMultiplicative =	m_pConfig->pBool("is_multiplicative_vertex");
	m_iNumReflections =		m_pConfig->pInt("num_reflex");

	m_iMaxPrimsNode =		m_pConfig->pInt("bvh_max_prims_node");
	m_sBVHSplit =			m_pConfig->pString("bvh_split_algorithm");

	m_fSpeed =				m_pConfig->pDouble("speed");

	m_sModel =				m_pConfig->pString("model_file");
	m_sAccelerationStruct=	m_pConfig->pString("acceleration_structure");

	m_iTextureWidth =		m_pConfig->pInt("texture_width");
	m_iTextureHeight =		m_pConfig->pInt("texture_height");
	m_iScreenMultiplier =	m_pConfig->pInt("screen_multiplier");

	m_iSampleCountX =		m_pConfig->pInt("sample_count_x");
	m_iSampleCountY =		m_pConfig->pInt("sample_count_y");

	m_iGroupSizeX =			m_pConfig->pInt("cs_group_size_x");
	m_iGroupSizeY =			m_pConfig->pInt("cs_group_size_y");
	m_iGroupSizeZ =			m_pConfig->pInt("cs_group_size_z");

	m_iLBVHDepth=			m_pConfig->pInt("lbvh_depth");

	m_iIterations =			m_pConfig->pInt("iterations");

	m_iRDThreadCount =		m_pConfig->pInt("rd_thread_count");
	m_iRDAddPixelCount =	m_pConfig->pInt("rd_add_pixel_count");

	return 1;
}

int	ArgumentsParser::LoadConfigurationFromFile(const string sFile)
{
	char* envp[] = {NULL};
	m_pConfig = new	Config(sFile,envp);

	return 0;
}

void ArgumentsParser::ShowConfiguration(unsigned int uiNumProcesses)
{
	//std::stringstream tmp ( std::stringstream::in | std::stringstream::out ); 
	cout << "\n---------------------------------------"
		<< "\nINI FILE"
		<< "\n---------------------------------------"
		<< "\nNumber of processes:\t"<< uiNumProcesses
		<< "\nNumber of threads:\t"<< m_iNumThreads
		<< "\nAcceleration struc.:\t"<< m_sAccelerationStruct
		<< "\nIs reflective:\t\t" << m_bIsReflective
		<< "\nIs Multiplicative:\t"<< m_bIsMultiplicative
		<< "\n# of Reflections:\t" << m_iNumReflections
		<< "\nMax Prims Node:\t\t" << m_iMaxPrimsNode
		<< "\nSplit Algorithm:\t" << m_sBVHSplit
		<< "\nSpeed:\t\t\t" << m_fSpeed
		<< "\nModel:\t\t\t" << m_sModel
		<< "\nTexture width:\t\t" << m_iTextureWidth
		<< "\nTexture height:\t\t" << m_iTextureHeight
		<< "\nScreen multiplier:\t" << m_iScreenMultiplier
		<< "\nIterations:\t\t" << m_iIterations
		<< "\n---------------------------------------\n";
	//printf("%s\n",tmp.str().c_str());
}

//bool ArgumentsParser::on_pre_parse(Glib::OptionContext& context, Glib::OptionGroup& group)
//{
//	return Glib::OptionGroup::on_pre_parse(context, group);
//}
//
//bool ArgumentsParser::on_post_parse(Glib::OptionContext& context, Glib::OptionGroup& group)
//{
//	return Glib::OptionGroup::on_post_parse(context, group);
//}
//
//void ArgumentsParser::on_error(Glib::OptionContext& context, Glib::OptionGroup& group)
//{
//	std::cerr << "There was an error parsing the arguments." <<std::endl;
//	Glib::OptionGroup::on_error(context, group);
//}