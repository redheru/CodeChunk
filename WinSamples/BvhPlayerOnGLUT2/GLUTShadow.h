#ifndef GLUTSHADOW_HPP
#define GLUTSHADOW_HPP

namespace GLUT
{

    class GLUTWidget;               // �O���錾(���ݎQ�Ƃ�����)
	class GLUTShadow
	{
	private:
		int texWidth_;               // �e�N�X�`�����W
	    int texHeight_;              // �e�N�X�`�����W 
		bool isUseAlpha_;            // �A���t�@�e�X�g���g�����H

    	float  angle_;               // ���C�g����V���h�E�}�b�v�����͈͂̃p�[�X�y�N�e�B�u�͈̔͂̊p�x�@
    	float  nearDistance_;        // �p�[�X�y�N�e�B�u�̗̈�̌����ɋ߂��ʂ܂ł̋����@ 
	    float  farDistance_;         // �p�[�X�y�N�e�B�u�̗̈�̌����ɉ����ʂ܂ł̋����@ �@ 
    	bool isLoaded_;
	    bool isInitialized_;
        bool isEnabled_;
    	//ViewGL* view_P;
	    GLUTWidget* glutWidget_;     //glutWidget 
	public:
    	//void ( *scene_P ) ();// �V�[���������_�����O����֐��ւ̃|�C���^
	    GLUTShadow();    // Shadow�N���X�̃R���X�g���N�^
		void Load(GLUTWidget* glutWidget, bool isUseAlpha = true, float angle = 60, float near = 2.0, float far = 20.0 );
		void drawScene();
		void Initialize();

    // >�A�N�Z�b�T
		int getTexWidth(){return texWidth_;}
		int getTexHeight(){return texHeight_;}
		bool getIsUseAlpha(){return isUseAlpha_;}
		float getAngle(){return angle_;}
		float getNearDistance(){return nearDistance_;}
		float getFarDistance(){return farDistance_;}
		bool getIsLoaded(){return isLoaded_;}
		bool getIsInitialized(){return isInitialized_;}
		bool getIsEnagled(){return isEnabled_;}

	// >�~���[�e�[�^
		void setTexWidth(int texWidth){texWidth_ = texWidth;}
		int setTexHeight(int texHeight){texHeight_ = texHeight;}
		bool setIsUseAlpha(bool isUseAlpha){isUseAlpha_ = isUseAlpha;}
		float setAngle(float angle){angle_ = angle;}
		float setNearDistance(float nearDistance){nearDistance_ = nearDistance;}
		float setFarDistance(float farDistance){farDistance_ = farDistance;}
		void setIsEnabled(bool isEnabled){isEnabled_ = isEnabled;}

	private:
		void Init();
		void createShadowMap(void);          // ���䂳��̎������Q�l�ɍ쐬�B3�p�X�ł̃V���h�E�}�b�v������

	}; // end class Shadow
} // end namespace GLUT

#endif