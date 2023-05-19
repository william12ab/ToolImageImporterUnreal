// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
class TOOLIMAGEIMPORTER_API SoundHandler
{
public:
	SoundHandler();
	~SoundHandler();

	void ClickSound();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* button_sound_cue;
	UAudioComponent* button_audio_comp;
};
