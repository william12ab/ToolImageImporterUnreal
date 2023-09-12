
#include "PaceNotesActor.h"

#include "UObject/ConstructorHelpers.h"
APaceNotesActor::APaceNotesActor(){
	PrimaryActorTick.bCanEverTick = true;
	note_count = 0;
	play_value = 0.25f;
	is_played = false;
	is_playing = false;
	static ConstructorHelpers::FObjectFinder<USoundCue> hobj(TEXT("SoundCue'/Game/Sound/pacenotes/100_Cue.100_Cue'"));
	if (hobj.Succeeded()) {
		hunder_cue = hobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> hfobj(TEXT("SoundCue'/Game/Sound/pacenotes/150_Cue.150_Cue'"));
	if (hfobj.Succeeded()) {
		hunderf_cue = hfobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> thobj(TEXT("SoundCue'/Game/Sound/pacenotes/200_Cue.200_Cue'"));
	if (thobj.Succeeded()) {
		twohunder_cue = thobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> olobj(TEXT("SoundCue'/Game/Sound/pacenotes/1_left_Cue.1_left_Cue'"));
	if (olobj.Succeeded()) {
		one_l_cue = olobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> orobj(TEXT("SoundCue'/Game/Sound/pacenotes/1_right_Cue.1_right_Cue'"));
	if (orobj.Succeeded()) {
		one_r_cue = orobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> tobj(TEXT("SoundCue'/Game/Sound/pacenotes/2_left_Cue.2_left_Cue'"));
	if (tobj.Succeeded()) {
		two_l_cue = tobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> trobj(TEXT("SoundCue'/Game/Sound/pacenotes/2_right_Cue.2_right_Cue'"));
	if (trobj.Succeeded()) {
		two_r_cue = trobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> thlobj(TEXT("SoundCue'/Game/Sound/pacenotes/3_left_Cue.3_left_Cue'"));
	if (thlobj.Succeeded()) {
		three_l_cue = thlobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> throbj(TEXT("SoundCue'/Game/Sound/pacenotes/3_right_Cue.3_right_Cue'"));
	if (throbj.Succeeded()) {
		three_r_cue = throbj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> fobj(TEXT("SoundCue'/Game/Sound/pacenotes/4_left_Cue.4_left_Cue'"));
	if (fobj.Succeeded()) {
		four_l_cue = fobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> frobj(TEXT("SoundCue'/Game/Sound/pacenotes/4_right_Cue.4_right_Cue'"));
	if (frobj.Succeeded()) {
		four_r_cue = frobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> fvobj(TEXT("SoundCue'/Game/Sound/pacenotes/5_left_Cue.5_left_Cue'"));
	if (fvobj.Succeeded()) {
		five_l_cue = fvobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> fvrobj(TEXT("SoundCue'/Game/Sound/pacenotes/5_right_Cue.5_right_Cue'"));
	if (fvrobj.Succeeded()) {
		five_r_cue = fvrobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> sobj(TEXT("SoundCue'/Game/Sound/pacenotes/6_left_Cue.6_left_Cue'"));
	if (sobj.Succeeded()) {
		six_l_cue = sobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> srobj(TEXT("SoundCue'/Game/Sound/pacenotes/6_right_Cue.6_right_Cue'"));
	if (srobj.Succeeded()) {
		six_r_cue = srobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> hplobj(TEXT("SoundCue'/Game/Sound/pacenotes/hairpin_left_Cue.hairpin_left_Cue'"));
	if (hplobj.Succeeded()) {
		hp_l_cue = hplobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> hprobj(TEXT("SoundCue'/Game/Sound/pacenotes/hairpin_right_Cue.hairpin_right_Cue'"));
	if (hprobj.Succeeded()) {
		hp_r_cue = hprobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> sqobj(TEXT("SoundCue'/Game/Sound/pacenotes/hard_left_Cue.hard_left_Cue'"));
	if (sqobj.Succeeded()) {
		sq_l_cue = sqobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> sqrobj(TEXT("SoundCue'/Game/Sound/pacenotes/hard_right_Cue.hard_right_Cue'"));
	if (sqrobj.Succeeded()) {
		sq_r_cue = sqrobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> intobj(TEXT("SoundCue'/Game/Sound/pacenotes/into_Cue.into_Cue'"));
	if (intobj.Succeeded()) {
		into_cue = intobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> andobj(TEXT("SoundCue'/Game/Sound/pacenotes/and_Cue.and_Cue'"));
	if (andobj.Succeeded()) {
		and_cue = andobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> tightenobj(TEXT("SoundCue'/Game/Sound/pacenotes/tightens_Cue.tightens_Cue'"));
	if (tightenobj.Succeeded()) {
		tighten_cue = tightenobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> shortobj(TEXT("SoundCue'/Game/Sound/pacenotes/short_Cue.short_Cue'"));
	if (shortobj.Succeeded()) {
		short_cue = shortobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> restobj(TEXT("SoundCue'/Game/Sound/pacenotes/over_crest_Cue.over_crest_Cue'"));
	if (restobj.Succeeded()) {
		crest_cue = restobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> openobj(TEXT("SoundCue'/Game/Sound/pacenotes/opens_Cue.opens_Cue'"));
	if (openobj.Succeeded()) {
		open_cue = openobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> narrowobj(TEXT("SoundCue'/Game/Sound/pacenotes/narrow_archway_Cue.narrow_archway_Cue'"));
	if (narrowobj.Succeeded()) {
		narrow_cue = narrowobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> longobj(TEXT("SoundCue'/Game/Sound/pacenotes/long_Cue.long_Cue'"));
	if (longobj.Succeeded()) {
		long_cue = longobj.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> dipobj(TEXT("SoundCue'/Game/Sound/pacenotes/dip_Cue.dip_Cue'"));
	if (dipobj.Succeeded()) {
		dip_cue = dipobj.Object;
	}
	current_comp = CreateDefaultSubobject<UAudioComponent>(TEXT("currents"));
	current_comp->SetupAttachment(RootComponent);
}

void APaceNotesActor::BeginPlay(){
	Super::BeginPlay();
}

void APaceNotesActor::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	Playing(DeltaTime);
}
void APaceNotesActor::FindDirection(const int& i, const int&n, const int& a) {
	if (directions_[i]>0){//right
		pacenotes_array[n] = a + 50;
	}
	else if (directions_[i] < 0){//left
		pacenotes_array[n] = a + 40;
	}
	else if (directions_[i] == 0){//same line
		//same line
	}
	//checks direction, modis most recent note, and changes for left and right
}
void APaceNotesActor::PlayNote(const float& DeltaTime, const int& index) {
	accum_time += DeltaTime;
	if (!is_playing){
		current_comp->Activate();
		current_comp->SetSound(cues_[0]);
		current_comp->Play(0.f);
		is_playing = true;
	}
	if (accum_time > cues_[0]->GetDuration()) {
		to_play.RemoveAt(0);
		cues_.RemoveAt(0);
		accum_time = 0.0f;
		is_playing = false;
	}
}

void APaceNotesActor::Playing(float DeltaTime){
	if (to_play.Num()!=0){
		PlayNote(DeltaTime,0);
	}
}

void APaceNotesActor::FindAngle(const int& i) {
	if (angles_[i] >= 0 && angles_[i] < 20) {
		pacenotes_array.Add(16);
		FindDirection(i, pacenotes_array.Num()-1, 16);
	}
	else if (angles_[i] >= 20 && angles_[i] < 35) {
		pacenotes_array.Add(15);
		FindDirection(i, pacenotes_array.Num() - 1, 15);
	}
	else if (angles_[i] >= 35 && angles_[i] < 50) {
		pacenotes_array.Add(14);
		FindDirection(i, pacenotes_array.Num() - 1, 14);
	}
	else if (angles_[i] >= 50 && angles_[i] < 65) {
		pacenotes_array.Add(13);
		FindDirection(i, pacenotes_array.Num() - 1, 13);
	}
	else if (angles_[i] >= 65 && angles_[i] < 75) {
		pacenotes_array.Add(12);
		FindDirection(i, pacenotes_array.Num() - 1, 12);
	}
	else if (angles_[i] >= 75 && angles_[i] < 89) {
		pacenotes_array.Add(11);
		FindDirection(i, pacenotes_array.Num() - 1, 11);
	}
	else if (angles_[i] >= 89 && angles_[i]<91) {
		pacenotes_array.Add(10);
		FindDirection(i, pacenotes_array.Num() - 1, 10);
	}
	else if(angles_[i] >= 91 ) {
		pacenotes_array.Add(9);
		FindDirection(i, pacenotes_array.Num() - 1, 9);
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

void APaceNotesActor::WhenToPlay(const FVector2D&p1, const FVector2D& p2, const FVector2D& p3) {
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
	if (t> play_value){
		if (!is_played){
			if (note_count<pacenotes_array.Num()){
				PlayNextNote();
				PlayAddition();
				note_count++;
				is_played = true;
			}
		}
	}
}

void APaceNotesActor::PlayNextNote() {
	UE_LOG(LogTemp, Warning, TEXT("NOTE: %d"),pacenotes_array[note_count]);
	switch (pacenotes_array[note_count])
	{
	case 16: {//l6
		to_play.Add(16);
		cues_.Add(six_l_cue);
		break;
	}
	case 1: {//100
		to_play.Add(1);
		cues_.Add(hunder_cue);
		note_count++;
		PlayNextNote();
		break;
	}
	case 2: {//150
		to_play.Add(2);
		cues_.Add(hunderf_cue);
		note_count++;
		PlayNextNote();
		break;
	}
	case 3: {//200
		cues_.Add(twohunder_cue);
		to_play.Add(3);
		PlayNextNote();
		note_count++;
		break;
	}
	case 56: {//l6
		to_play.Add(56);
		cues_.Add(six_l_cue);
		break;
	}
	case 55: {//l5
		to_play.Add(55);
		cues_.Add(five_l_cue);
		break;
	}
	case 54: {//l4
		to_play.Add(54);
		cues_.Add(four_l_cue);
		break;
	}
	case 53: {//l3
		to_play.Add(53);
		cues_.Add(three_l_cue);
		break;
	}
	case 52: {//l2
		to_play.Add(52);
		cues_.Add(two_l_cue);
		break;
	}
	case 51: {//l1
		to_play.Add(51);
		cues_.Add(one_l_cue);
		break;
	}
	case 50: {//sql
		to_play.Add(50);
		cues_.Add(sq_l_cue);
		break;
	}
	case 59: {//hpl
		to_play.Add(59);
		cues_.Add(hp_l_cue);
		break;
	}
	case 66: {//r6
		to_play.Add(66);
		cues_.Add(six_r_cue);
		break;
	}
	case 65: {//r5
		to_play.Add(65);
		cues_.Add(five_r_cue);
		break;
	}
	case 64: {//r4
		to_play.Add(64);
		cues_.Add(four_r_cue);
		break;
	}
	case 63: {//r3
		to_play.Add(63);
		cues_.Add(three_r_cue);
		break;
	}
	case 62: {//r2
		to_play.Add(62);
		cues_.Add(two_r_cue);
		break;
	}
	case 61: {//r1
		to_play.Add(61);
		cues_.Add(one_r_cue);
		break;
	}
	case 60: {//sqr
		to_play.Add(60);
		cues_.Add(sq_r_cue);
		break;
	}
	case 69: {//hpr
		to_play.Add(69);
		cues_.Add(hp_r_cue);
		break;
	}
	}
}

void APaceNotesActor::PlayAddition() {
	bool is_addition = false;
	UE_LOG(LogTemp, Warning, TEXT("NOTEaddition: %d"), pacenotes_array[note_count]);
	if (note_count + 1 < pacenotes_array.Num()) {
		switch (pacenotes_array[note_count + 1])
		{
		case 20: {
			is_addition = true;
			//tightens/widens
			to_play.Add(20);
			cues_.Add(tighten_cue);
			note_count++;
			break;
		}
		case 21: {
			is_addition = true;
			//into
			to_play.Add(21);
			cues_.Add(into_cue);
			note_count++;
			break;
		}
		case 22: {
			is_addition = true;
			//and
			to_play.Add(22);
			cues_.Add(and_cue);
			note_count++;
			break;
		}
		}
	}
	if (is_addition){
		note_count++;
		PlayNextNote();
	}
}