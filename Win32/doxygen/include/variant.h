#ifndef H_SPK_NORMALEMITTER
#define H_SPK_NORMALEMITTER

#include "Core/SPK_Emitter.h"

/**
* @mainpage SPARK Particle Engine
*
* SPARK is an open source library allowing to easily implement full particle systems in C++ applications.<br>
* <br>
* SPARK has been designed to be :
* <ul>
* <li>user friendly and easy to implement</li>
* <li>very configurable and as complete as possible</li>
* <li>evolving and upgradable by users</li>
* <li>portable and library free (it only uses the standard library)</li>
* </ul>
* <br>
* @section intro_sec Global Description
* in SPARK, a SPK::System is a collection of Groups of Particles.
* A SPK::Group contains a SPK::Pool of Particles and defines an complete universe for Particle evolution.
* This universe holds 3 types of physical objects :
* <ul>
* <li>the Emitters : an SPK::Emitter is an object that generates Particles</li>
* <li>the Modifiers : a SPK::Modifier is an object that will modify Particles behavior</li>
* <li>the Particles themselves : a SPK::Particle is a point that will follow physical laws and will be given some parameters that will evolve from their birth to their death.</li>
* </ul>
* Those parameters are defined by a SPK::Model of Particles.<br>
* The Emitters and Modifiers are physical entities whose body is represented by a SPK::Zone.<br>
* A SPK::Vector3D is the primitive object used in SPARK to represents triplets of coordinates in a universe 3D.<br>
* <br>
* Finally a system/groups can be renderered using a SPK::Renderer.
*/

/**
* @namespace SPK
* @brief the namespace for the whole SPARK library
*/
namespace SPK
{

	

	/**
	* @class variant
	* @brief Variant is a container that can store a selection of basic types. 
	*
	* The variant will store the value in the native form corresponding to the version of Set that was called.<br>
	* Get is templated to convert from the stored form to the requested form by using a TypeConverter.
	*
	* @since 1.02.00
	*/
	class variant
	{
	private:

		//TODO 确认位数
#ifdef YON_ARCH_BITMODE_64
		static const s32 LOCAL_DATA_SIZE = 32; // Required for Strings
#else
		static const s32 LOCAL_DATA_SIZE = 24;
#endif
		ENUM_TYPE Type;
		c8 Data[LOCAL_DATA_SIZE];
	public :

		/**
		* @enum Type
		* @brief Type of data stored in the variant
		*/
		enum Type
		{
			NONE = '-',						/**< Type of 1 */ 
			BYTE = 'b',						/**< Type of 2 */ 
			CHAR = 'c',						/**< Type of 3 */ 
			FLOAT = 'f',					/**< Type of 4 */ 
			INT = 'i',						/**< Type of 5 */ 
			STRING = 's',					/**< Type of 6 */ 
			WORD = 'w',						/**< Type of 7 */ 
			VECTOR2 = '2',					/**< Type of 8 */ 
			COLOURF = 'g',					/**< Type of 9 */ 
			COLOURB = 'h',					/**< Type of 10 */ 
			VOIDPTR = '*'					/**< Type of 11 */ 
		};


		/**
		 * @brief Constructor of variant
		 */
		variant();

		/**
		 * @brief Copy Constructor of variant
		 *
		 * Templatised constructors don't work for the copy constructor, so we have to define it explicitly
		 */
		variant(const variant&);

		/**
		 * @brief Constructs a variant with internal data.
		 * @param[in] t Data of a supported type to set on the variant.
		 */
		template< typename T >
		variant(const T& t);

		/**
		* 设置日期的显存
		* @param[in] beginPos 对应区域开始显示的地址
		* @param[in] order order>0: year/month/date;order=0: date/month/year
		* @param[in] linkChar 日期间的连接符, 目前仅支持 '*''#''-''_''='
		* @param[in] year 1-9999
		* @param[in] month 1-12
		* @param[in] day 1-31
		* @return 操作结果, 见 ERR_LED_E
		* @note 只有在调用 LED_Update 后才起效
		* @see ::LED_Update ::LED_SetSerialBuf ::LED_SetCharBuf 
		* @see ::LED_SetTimeBuf ::LED_SetMoneyBuf ::LED_ClearAreaBuf
		*/ 
		void setInverted(bool inverted);

		/**
		* @brief Tells whether normals are inverted for this NormalEmitter
		* @return true if normals are inverted, false if not
		*/
		bool isInverted() const;

		/**
		* @brief Gets the normal Zone of this NormalEmitter
		* @return the normal Zone of this NormalEmitter
		*/
		Zone* getNormalZone() const;
	};

}

#endif
