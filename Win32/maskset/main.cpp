#include <stdio.h>

enum MASK_TEXTURE_CREATION_CONFIG{
	MASK_TEXTURE_CREATION_CONFIG_NONE			= 0,
	MASK_TEXTURE_CREATION_CONFIG_RESERVE_IMAGE	= 0x00000001,
	MASK_TEXTURE_CREATION_CONFIG_16BIT			= 0x00000002,//”ÎMASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHAª•≥‚
	MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA	= 0x00000004,//”ÎMASK_TEXTURE_CREATION_CONFIG_16BITª•≥‚
	MASK_TEXTURE_CREATION_CONFIG_MIPMAPS		= 0x00000008
};

typedef unsigned int u32;

u32 m_textureCreationConfig;

void setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG config,bool on){
	m_textureCreationConfig = (m_textureCreationConfig & (~config)) |((((u32)!on)-1) & config);
}

int main(int argc, char* argv[])
{
	m_textureCreationConfig=MASK_TEXTURE_CREATION_CONFIG_NONE;
	setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_RESERVE_IMAGE,true);
	printf("%d\n",m_textureCreationConfig);
	setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_16BIT,true);
	printf("%d\n",m_textureCreationConfig);
	setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_MIPMAPS,true);
	printf("%d\n",m_textureCreationConfig);
	setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_RESERVE_IMAGE,false);
	printf("%d\n",m_textureCreationConfig);
	setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_16BIT,false);
	printf("%d\n",m_textureCreationConfig);
	setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_MIPMAPS,false);
	printf("%d\n",m_textureCreationConfig);
	getchar();
}