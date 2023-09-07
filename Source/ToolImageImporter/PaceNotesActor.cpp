
#include "PaceNotesActor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
APaceNotesActor::APaceNotesActor(){
	PrimaryActorTick.bCanEverTick = true;
	note_count = 0;
	static ConstructorHelpers::FObjectFinder<USoundCue> hobj(TEXT("SoundCue'/Game/Sound/pacenotes/100_Cue.100_Cue'"));
	if (hobj.Succeeded()) {
		hunder_cue = hobj.Object;
		hunder_comp = CreateDefaultSubobject<UAudioComponent>(TEXT("hundercomp"));
		hunder_comp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> hfobj(TEXT("SoundCue'/Game/Sound/pacenotes/150_Cue.150_Cue'"));
	if (hfobj.Succeeded()) {
		hunderf_cue = hfobj.Object;
		hunderf_comp = CreateDefaultSubobject<UAudioComponent>(TEXT("hunderfcomp"));
		hunderf_comp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> thobj(TEXT("SoundCue'/Game/Sound/pacenotes/200_Cue.200_Cue'"));
	if (thobj.Succeeded()) {
		twohunder_cue = thobj.Object;
		thunder_comp = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundComp"));
		thunder_comp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> olobj(TEXT("SoundCue'/Game/Sound/pacenotes/1_left_Cue.1_left_Cue'"));
	if (olobj.Succeeded()) {
		one_l_cue = olobj.Object;
		olcomp= CreateDefaultSubobject<UAudioComponent>(TEXT("ol"));
		olcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> orobj(TEXT("SoundCue'/Game/Sound/pacenotes/1_right_Cue.1_right_Cue'"));
	if (orobj.Succeeded()) {
		one_r_cue = orobj.Object;
		orcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("or"));
		orcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> tobj(TEXT("SoundCue'/Game/Sound/pacenotes/2_left_Cue.2_left_Cue'"));
	if (tobj.Succeeded()) {
		two_l_cue = tobj.Object;
		tlcomp= CreateDefaultSubobject<UAudioComponent>(TEXT("tl"));
		tlcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> trobj(TEXT("SoundCue'/Game/Sound/pacenotes/2_right_Cue.2_right_Cue'"));
	if (trobj.Succeeded()) {
		two_r_cue = trobj.Object;
		trcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("tr"));
		trcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> thlobj(TEXT("SoundCue'/Game/Sound/pacenotes/3_left_Cue.3_left_Cue'"));
	if (thlobj.Succeeded()) {
		three_l_cue = thlobj.Object;
		thlcomp= CreateDefaultSubobject<UAudioComponent>(TEXT("thl"));
		thlcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> throbj(TEXT("SoundCue'/Game/Sound/pacenotes/3_right_Cue.3_right_Cue'"));
	if (throbj.Succeeded()) {
		three_r_cue = throbj.Object;
		thrcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("thr"));
		thrcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> fobj(TEXT("SoundCue'/Game/Sound/pacenotes/4_left_Cue.4_left_Cue'"));
	if (fobj.Succeeded()) {
		four_l_cue = fobj.Object;
		flcomp= CreateDefaultSubobject<UAudioComponent>(TEXT("fl"));
		flcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> frobj(TEXT("SoundCue'/Game/Sound/pacenotes/4_right_Cue.4_right_Cue'"));
	if (frobj.Succeeded()) {
		four_r_cue = frobj.Object;
		frcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("fr"));
		frcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> fvobj(TEXT("SoundCue'/Game/Sound/pacenotes/5_left_Cue.5_left_Cue'"));
	if (fvobj.Succeeded()) {
		five_l_cue = fvobj.Object;
		fvlcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("fv"));
		fvlcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> fvrobj(TEXT("SoundCue'/Game/Sound/pacenotes/5_right_Cue.5_right_Cue'"));
	if (fvrobj.Succeeded()) {
		five_r_cue = fvrobj.Object;
		fvrcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("fvr"));
		fvrcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> sobj(TEXT("SoundCue'/Game/Sound/pacenotes/6_left_Cue.6_left_Cue'"));
	if (sobj.Succeeded()) {
		six_l_cue = sobj.Object;
		slcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("sl"));
		slcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> srobj(TEXT("SoundCue'/Game/Sound/pacenotes/6_right_Cue.6_right_Cue'"));
	if (srobj.Succeeded()) {
		six_r_cue = srobj.Object;
		srcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("sr"));
		srcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> hplobj(TEXT("SoundCue'/Game/Sound/pacenotes/hairpin_left_Cue.hairpin_left_Cue'"));
	if (hplobj.Succeeded()) {
		hp_l_cue = hplobj.Object;
		hplcomp= CreateDefaultSubobject<UAudioComponent>(TEXT("hpl"));
		hplcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> hprobj(TEXT("SoundCue'/Game/Sound/pacenotes/hairpin_right_Cue.hairpin_right_Cue'"));
	if (hprobj.Succeeded()) {
		hp_r_cue = hprobj.Object;
		hprcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("hpr"));
		hprcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> sqobj(TEXT("SoundCue'/Game/Sound/pacenotes/hard_left_Cue.hard_left_Cue'"));
	if (sqobj.Succeeded()) {
		sq_l_cue = sqobj.Object;
		sqlcomp= CreateDefaultSubobject<UAudioComponent>(TEXT("sql"));
		sqlcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> sqrobj(TEXT("SoundCue'/Game/Sound/pacenotes/hard_right_Cue.hard_right_Cue'"));
	if (sqrobj.Succeeded()) {
		sq_r_cue = sqrobj.Object;
		sqrcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("sqr"));
		sqrcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> intobj(TEXT("SoundCue'/Game/Sound/pacenotes/into_Cue.into_Cue'"));
	if (intobj.Succeeded()) {
		into_cue = intobj.Object;
		intocomp = CreateDefaultSubobject<UAudioComponent>(TEXT("intoc"));
		intocomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> andobj(TEXT("SoundCue'/Game/Sound/pacenotes/and_Cue.and_Cue'"));
	if (andobj.Succeeded()) {
		and_cue = andobj.Object;
		andcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("andc"));
		andcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> tightenobj(TEXT("SoundCue'/Game/Sound/pacenotes/tightens_Cue.tightens_Cue'"));
	if (tightenobj.Succeeded()) {
		tighten_cue = tightenobj.Object;
		tightencomp = CreateDefaultSubobject<UAudioComponent>(TEXT("tightenc"));
		tightencomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> shortobj(TEXT("SoundCue'/Game/Sound/pacenotes/short_Cue.short_Cue'"));
	if (shortobj.Succeeded()) {
		short_cue = shortobj.Object;
		shortcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("shortcom"));
		shortcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> restobj(TEXT("SoundCue'/Game/Sound/pacenotes/over_crest_Cue.over_crest_Cue'"));
	if (restobj.Succeeded()) {
		crest_cue = restobj.Object;
		crestcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("cresto"));
		crestcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> openobj(TEXT("SoundCue'/Game/Sound/pacenotes/opens_Cue.opens_Cue'"));
	if (openobj.Succeeded()) {
		open_cue = openobj.Object;
		opencomp = CreateDefaultSubobject<UAudioComponent>(TEXT("openio"));
		opencomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> narrowobj(TEXT("SoundCue'/Game/Sound/pacenotes/narrow_archway_Cue.narrow_archway_Cue'"));
	if (narrowobj.Succeeded()) {
		narrow_cue = narrowobj.Object;
		narrowcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundComp"));
		narrowcomp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> longobj(TEXT("SoundCue'/Game/Sound/pacenotes/long_Cue.long_Cue'"));
	if (longobj.Succeeded()) {
		long_cue = longobj.Object;
		longcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundComp"));
		longcomp->SetupAttachment(RootComponent);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> dipobj(TEXT("SoundCue'/Game/Sound/pacenotes/dip_Cue.dip_Cue'"));
	if (dipobj.Succeeded()) {
		dip_cue = dipobj.Object;
		dipcomp = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundComp"));
		dipcomp->SetupAttachment(RootComponent);
	}
}

void APaceNotesActor::BeginPlay(){
	Super::BeginPlay();
}

void APaceNotesActor::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void APaceNotesActor::FindAngle(const int& i) {
	if (angles_[i] >= 0 && angles_[i] < 20) {
		pacenotes_array.Add(16);
	}
	else if (angles_[i] >= 20 && angles_[i] < 35) {
		pacenotes_array.Add(15);
	}
	else if (angles_[i] >= 35 && angles_[i] < 50) {
		pacenotes_array.Add(14);
	}
	else if (angles_[i] >= 50 && angles_[i] < 65) {
		pacenotes_array.Add(13);
	}
	else if (angles_[i] >= 65 && angles_[i] < 75) {
		pacenotes_array.Add(12);
	}
	else if (angles_[i] >= 75 && angles_[i] < 89) {
		pacenotes_array.Add(11);
	}
	else if (angles_[i] >= 89 && angles_[i]<91) {
		pacenotes_array.Add(10);
	}
	else if(angles_[i] >= 91 ) {
		pacenotes_array.Add(9);
	}
}

void APaceNotesActor::FindOrder() {
	for (size_t i = 0; i < lengths_.Num(); i++){
		//lengths 150,100,200
		if (lengths_[i] > (45) && lengths_[i] < (70)) {
			pacenotes_array.Add(1);
		}
		else if (lengths_[i] >= (70) && lengths_[i] < (95)) {
			pacenotes_array.Add(2);
		}
		else if (lengths_[i] >= (95) && lengths_[i] < (105)) {
			pacenotes_array.Add(3);
		}

		//angles
		if (i<(angles_.Num())){
			FindAngle(i);
			//checking if next length is short
			bool is_added = false;
			if (lengths_[i+1]<5){
				pacenotes_array.Add(20);//tighten/widen
				is_added = true;
			}
			else if (lengths_[i+1]>=5 && lengths_[i + 1]<15){
				pacenotes_array.Add(21);//into
				is_added = true;
			}
			else if (lengths_[i + 1] >= 15 && lengths_[i + 1] < 25) {
				pacenotes_array.Add(22);//and
				is_added = true;
			}
			//find next angle too
			if (is_added ){
				if ((i+1)<angles_.Num()){
					FindAngle(i + 1);
					i++;
				}
			}
		}
	}
}

void APaceNotesActor::WhenToPlay(const FVector&p1, const FVector& p2, const FVector& p3) {
	auto a = p1 - p3;
	auto b = p1 - p2;

	float t = 0.0f;
	if (a.X == 0 || b.X == 0){
		float c2 = ((float)a.Y / (float)b.Y);
		t = c2;
		if (t < 0.0f){
			t *= -1;
		}
	}
	else if (a.Y == 0 || b.Y == 0){
		float c1 = ((float)a.X/ (float)b.X);
		t = c1;
	}
	else{
		float c1 = ((float)a.X / (float)b.X);
		float c2 = ((float)a.Y / (float)b.Y);
		t = (c1 + c2) / 2.0f;
	}
	//above works out t

	if (t< play_value){
		PlayNextNote();
		PlayAddition();
		note_count++;
	}
}

void APaceNotesActor::PlayNextNote() {
	switch (pacenotes_array[note_count])
	{
	case 1: {
		hunder_comp->Activate(true);
		hunder_comp->SetSound(hunder_cue);
		hunder_comp->Play(0.f);
		break;
	}
	case 2: {
		hunderf_comp->Activate(true);
		hunderf_comp->SetSound(hunderf_cue);
		hunderf_comp->Play(0.f);
		break;
	}
	case 3: {
		thunder_comp->Activate(true);
		thunder_comp->SetSound(twohunder_cue);
		thunder_comp->Play(0.f);
		break;
	}
	case 16: {
		thunder_comp->Activate(true);
		thunder_comp->SetSound(twohunder_cue);
		thunder_comp->Play(0.f);
		break;
	}
	case 15: {
		thunder_comp->Activate(true);
		thunder_comp->SetSound(twohunder_cue);
		thunder_comp->Play(0.f);
		break;
	}
	case 14: {
		thunder_comp->Activate(true);
		thunder_comp->SetSound(twohunder_cue);
		thunder_comp->Play(0.f);
		break;
	}
	case 13: {
		thunder_comp->Activate(true);
		thunder_comp->SetSound(twohunder_cue);
		thunder_comp->Play(0.f);
		break;
	}
	case 12: {
		thunder_comp->Activate(true);
		thunder_comp->SetSound(twohunder_cue);
		thunder_comp->Play(0.f);
		break;
	}
	case 11: {
		thunder_comp->Activate(true);
		thunder_comp->SetSound(twohunder_cue);
		thunder_comp->Play(0.f);
		break;
	}
	case 10: {
		thunder_comp->Activate(true);
		thunder_comp->SetSound(twohunder_cue);
		thunder_comp->Play(0.f);
		break;
	}
	case 9: {
		thunder_comp->Activate(true);
		thunder_comp->SetSound(twohunder_cue);
		thunder_comp->Play(0.f);
		break;
	}
	}
}

void APaceNotesActor::PlayAddition() {
	bool is_addition = false;
	switch (pacenotes_array[note_count])
	{
	case 20: {
		is_addition = true;
	}
	case 21: {
		is_addition = true;
	}
	case 22: {
		is_addition = true;
	}
	}
	if (is_addition){
		note_count++;
		PlayNextNote();
	}
}