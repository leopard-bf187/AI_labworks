// [LAB3][BOIDS] КАРТА КОДА: GPUBoids.fx содержит вычисления модели Boids и визуализацию.
// [LAB3][BOIDS] ПРАВИЛА: Seek(), Avoid(), Flee(), Separation(), Cohesion(), Alignment().
// [LAB3][BOIDS] ПОПАРНЫЕ СИЛЫ: PSForce() вызывается техникой AccumulateForces[GS].
// [LAB3][BOIDS] ОБНОВЛЕНИЕ АГЕНТОВ: PSAdvance() складывает силы и изменяет состояние.
// [LAB3][BOIDS] ОТРИСОВКА: VSBoids() читает состояние и размещает экземпляры модели.
// [LAB3][BOIDS] Вызовы этих техник и порядок проходов см. OnD3D10FrameRender() в .cpp.
// Structures for vertex input
struct VS_PARTICLE_IN
{
    float4   color            : COLOR;             //particle color
    uint	 id				  : SV_VERTEXID;       //sys-gen vertex id
};

struct VSSplatIn
{
    float3   pos              : POS;
    float2   tex			  : TEXCOORD0;
    uint	 id				  : SV_VERTEXID;       //sys-gen vertex id
};



// Structures for drawing particles
struct VS_PARTICLE_DRAW_OUT
{
    float3 pos			: POSITION;
    float4 color		: COLOR;
};

struct GS_PARTICLE_DRAW_OUT
{
    float2 tex			: TEXCOORD0;
    float4 color		: COLOR;
    float4 pos			: SV_POSITION;
};

struct PS_PARTICLE_DRAW_IN
{
    float2 tex			: TEXCOORD0;
	float4 color		: COLOR;
};



// Structures for accumulating forces
struct VS_FORCE_OUT
{
    float3 pos			: POS;
};

struct GS_FORCE_OUT
{
	float3 pos			: POS;
    float2 tex			: TEXCOORD0;
    float4 clippos		: SV_POSITION;
};

struct PS_FORCE_IN
{
    float3 pos			: POS;
    float2 tex			: TEXCOORD0;
};


// boids
struct VS_BOID_IN
{
	float3 pos			: POS;
	float3 norm			: NORMAL;
	float2 tex			: TEXCOORD0;
	uint instanceID		: SV_INSTANCEID;
};

struct VS_BOID_OUT
{
	float3 norm			: NORMAL;
	float4 pos			: SV_POSITION;
};

struct PS_BOID_IN
{
	float3 norm			: NORMAL;
};

// mesh
struct VS_MESH_IN
{
	float3 pos			: POS;
	float3 norm			: NORMAL;
	float2 tex			: TEXCOORD0;
};

struct VS_MESH_OUT
{
	float3 norm			: NORMAL;
	float2 tex			: TEXCOORD0;
	float4 pos			: SV_POSITION;
};

struct PS_MESH_IN
{
	float3 norm			: NORMAL;
	float2 tex			: TEXCOORD0;
};


// Structures for advancing the particles
struct VS_ADVANCE_OUT
{
    float3 pos			: POS;
};

struct GS_ADVANCE_OUT
{
    float2 tex			: TEXCOORD0;
    uint   irt			: RENDERTARGET;
    float4 clippos		: SV_POSITION;
    uint   rtindex		: SV_RENDERTARGETARRAYINDEX;
};

struct PS_ADVANCE_IN
{
    float2 tex			: TEXCOORD0;
    uint   irt			: RENDERTARGET;
};




// [LAB3][BOIDS] Матрицы для рендеринга и g_fElapsedTime для интегрирования движения.
cbuffer cb0
{
    float4x4 g_mWorldViewProj;
    float4x4 g_mWorld;
    float4x4 g_mInvView;
    float g_fGlobalTime;
    float g_fElapsedTime;
};

// [LAB3][BOIDS] Размер текстуры состояния, максимальный mip, положение шара Flee и масштаб mesh.
cbuffer cbRare
{
	uint  g_iTexSize;
    uint  g_iMaxMip;
    float3 g_vFleePos = float3(100,0,0);
    float g_fBoidScale = 1.0f;
};


// [LAB3][BOIDS] НАСТРОЙКИ ПОВЕДЕНИЯ: g_fMaxForce — верхняя граница суммарной силы;
// [LAB3][BOIDS] g_fMaxSpeed — предел скорости; *Strength — веса отдельных поведенческих сил.
// [LAB3][BOIDS] g_vSeekPos — точка притяжения; g_vFleePos (cbRare) — точка отпугивания.
cbuffer cb1
{
	float g_fParticleRad = 0.5f;
	float g_fParticleMass = 1.0f;
	float g_fG = 6.67300e-11;
	float g_fMaxForce = 60.0f;
	float g_fMaxSpeed = 25.0f;
	float g_fAvoidStrength = 5.0f;
	float g_fSeekStrength = 3.0f;
	//float g_fFleeStrength = 300000.0f;
	float g_fFleeStrength = 900000.0f;
	float g_fSeparationStrength = 1.0f;
	float g_fCohesionStrength = 0.5f;
	//float g_fAlignmentStrength = 1.0f;
	float g_fAlignmentStrength = 0.5f;
	float3 g_vSeekPos = float3(0,0,0);
	float3 g_vLightPos = float3( -0.707f, 0.707f, 0.0f );
};


// [LAB3][BOIDS] Четыре угла полноэкранного квадрата; нужны GS-проходам обновления и splat.
cbuffer cbImmutable
{
    float3 g_positions[4] =
    {
        float3( -1, 1, 0 ),
        float3( 1, 1, 0 ),
        float3( -1, -1, 0 ),
        float3( 1, -1, 0 ),
    };
    
    float2 g_texcoords[4] = 
    { 
        float2(0,0), 
        float2(1,0),
        float2(0,1),
        float2(1,1),
    };
};



Texture2D		g_txDiffuse;


// [LAB3][BOIDS] ГЛАВНЫЕ ДАННЫЕ АГЕНТОВ: array slice 0 = позиция, 1 = направление + скорость,
// [LAB3][BOIDS] 2 = вектор up. Texel (id % g_iTexSize, id / g_iTexSize) соответствует агенту id.
// [LAB3][BOIDS] g_txForce — Texture2D с накопленной силой Separation/Avoid на каждого агента.
Texture2DArray  g_txParticleData;
Texture2D		g_txForce;



SamplerState g_samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

SamplerState g_samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
};

BlendState ParticleBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = ONE;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

BlendState AdditiveBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    SrcBlend = ONE;
    DestBlend = ONE;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

BlendState AdvanceBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = FALSE;
    BlendEnable[1] = FALSE;
    SrcBlend = ONE;
    DestBlend = ONE;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
    RenderTargetWriteMask[1] = 0x0F;
};

BlendState NoBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = FALSE;
};

DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};



// Vertex shader for drawing the point-sprite particles
VS_PARTICLE_DRAW_OUT VSParticleDraw(VS_PARTICLE_IN input)
{
    VS_PARTICLE_DRAW_OUT output;
    
    // Lookup the particle position
    int4 texcoord;
    texcoord.x = input.id % g_iTexSize;
    texcoord.y = input.id / g_iTexSize;
    texcoord.z = 0; // position
    texcoord.w = 0; // mip level

    output.pos = g_txParticleData.Load(texcoord).xyz;
    
    float3 force = g_txForce.Load(texcoord).xyz;
    output.color = float4(1,0.5,0.2,1);
    
    return output;
}




// GS for rendering point sprite particles.  Takes a point and turns it into 2 tris.
[maxvertexcount(4)]
void GSParticleDraw(point VS_PARTICLE_DRAW_OUT input[1], inout TriangleStream<GS_PARTICLE_DRAW_OUT> SpriteStream)
{
	GS_PARTICLE_DRAW_OUT output;
    
    // Emit two new triangles
    for(int i=0; i<4; i++)
    {
        float3 position = g_positions[i]*g_fParticleRad;
        position = mul( position, (float3x3)g_mInvView ) + input[0].pos;
        output.pos = mul( float4(position,1.0), g_mWorldViewProj );
        
        output.color = input[0].color;
        output.tex = g_texcoords[i];
        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
}




// PS for drawing particles
float4 PSParticleDraw(PS_PARTICLE_DRAW_IN input) : SV_Target
{   
    return g_txDiffuse.Sample( g_samLinear, input.tex ) * input.color;
}



// Force accumulation
// [LAB3][BOIDS] Подготовка попарного прохода с GS: по SV_VERTEXID читаем позицию соседа.
VS_FORCE_OUT VSForceGS(VS_PARTICLE_IN input)
{
    VS_FORCE_OUT output;
    
    // Lookup the particle position
    int4 texcoord;
    texcoord.x = input.id % g_iTexSize;
    texcoord.y = input.id / g_iTexSize;
    texcoord.z = 0; // position
    texcoord.w = 0; // mip level

    output.pos = g_txParticleData.Load(texcoord).xyz;
    
    return output;
}


// Force accumulation
// [LAB3][BOIDS] Подготовка попарного прохода без GS: 6 вершин относятся к одному агенту.
// [LAB3][BOIDS] input.id / 6 позволяет восстановить id агента — источника взаимодействия.
GS_FORCE_OUT VSForce(VSSplatIn input)
{
    GS_FORCE_OUT output;
 
    // Lookup the particle position
    uint id = input.id / 6;
    int4 texcoord;
    texcoord.x = id % g_iTexSize;
    texcoord.y = id / g_iTexSize;
    texcoord.z = 0; // position
    texcoord.w = 0; // mip level

    output.pos = g_txParticleData.Load(texcoord).xyz;
    output.tex = input.tex;
    output.clippos = float4( input.pos, 1 );
    
    return output;
}


// Fullscreen GS for splatting forces
[maxvertexcount(4)]
// [LAB3][BOIDS] Разворачивает одну точку-источник в полноэкранный quad.
// [LAB3][BOIDS] Затем PSForce рассчитывает вклад этого источника в texel каждого другого boid.
void GSForce(point VS_FORCE_OUT input[1], inout TriangleStream<GS_FORCE_OUT> SpriteStream)
{
	GS_FORCE_OUT output;
    
    output.pos = input[0].pos;
    
    //
    // Emit two new triangles
    //
    for(int i=0; i<4; i++)
    {
        output.clippos = float4(g_positions[i],1);
        output.tex = g_texcoords[i];
        
        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
}



// Seek behavior
// [LAB3][BOIDS] SEEK: притяжение к цели g_vSeekPos. Вектор от текущей позиции к цели
// [LAB3][BOIDS] нормализуется, умножается на MaxSpeed; из желаемой скорости вычитается текущая.
// [LAB3][BOIDS] Результат — управляющая сила (steering), ещё без весового коэффициента.
float3 Seek( float3 vLocalPos, float3 vLocalVel, float3 vSeekPos )
{
	float3 vDelta = normalize( vSeekPos - vLocalPos );
	float3 vDesired = vDelta * g_fMaxSpeed;
	return (vDesired - vLocalVel);
}


// Avoid behavior
// [LAB3][BOIDS] AVOID: дополнительное отталкивание относительно линии текущего движения.
// [LAB3][BOIDS] Проецируем вектор до соседа на текущую траекторию, строим перпендикуляр
// [LAB3][BOIDS] и усиливаем отталкивание при уменьшении расстояний.
// [LAB3][BOIDS] Важно: сюда подаются позиции других boids, а не геометрия препятствий сцены.
float3 Avoid( float3 vLocalPos, float3 vLocalVel, float3 vAvoidPos )
{
	float3 vDir = normalize( vLocalVel );
	float3 vOP = vAvoidPos - vLocalPos;
	float t = dot( vOP, vDir );
	
	float3 vTPoint = vLocalPos + vDir*t;
	float3 vForceDir = vTPoint - vAvoidPos;
	float lenForceDir = length( vForceDir );
	vForceDir = normalize( vForceDir );
	float lenOP = length( vOP );
	
	lenForceDir /= g_fAvoidStrength;
	lenOP /= g_fAvoidStrength;
	
	float falloff1 = 1/(lenOP*lenOP);
	float falloff2 = 1/lenForceDir;
	
	return vForceDir * g_fMaxForce * falloff1 * falloff2;
}



// Flee behavior
// [LAB3][BOIDS] FLEE: бегство от управляемого шара. Направление противоположно вектору к шару;
// [LAB3][BOIDS] сила уменьшается приблизительно как 1/r^2 при удалении от него.
float3 Flee( float3 vLocalPos, float3 vLocalVel, float3 vFleePos )
{
	float3 vDelta = vLocalPos - vFleePos;
	float lenDelta = length( vDelta );
	vDelta /= lenDelta;
	float3 vDesired = vDelta * g_fMaxSpeed;
	return (vDesired - vLocalVel) / (lenDelta*lenDelta);
}


// Separation behavior
// [LAB3][BOIDS] SEPARATION (основное правило Boids): раздвигает близко расположенных агентов.
// [LAB3][BOIDS] Вектор от соседа к текущему boid нормализуется и умножается на MaxForce/r^2.
// [LAB3][BOIDS] Ограничения по радиусу соседства в исходнике нет: участвуют все агенты.
float3 Separation( float3 vLocalPos, float3 vNeighborPos )
{
	float3 vDelta = vLocalPos - vNeighborPos;
	float lenDelta = length( vDelta );
	vDelta /= lenDelta;
	
	return (g_fMaxForce * vDelta) / (lenDelta*lenDelta);
}


// Cohesion behavior
// [LAB3][BOIDS] COHESION (основное правило Boids): стремление к общему центру роя.
// [LAB3][BOIDS] Здесь vCenter — приближённое среднее по ВСЕЙ текстуре через старший mip,
// [LAB3][BOIDS] а не среднее локальных соседей, как в классической реализации Reynolds.
float3 Cohesion( float3 vLocalPos, float3 vLocalVel, float3 vCenter )
{
	float3 vDelta = normalize( vCenter - vLocalPos );
	return (g_fMaxForce * vDelta);
}


// Alignment behavior
// [LAB3][BOIDS] ALIGNMENT (основное правило Boids): согласование направления движения.
// [LAB3][BOIDS] Разница между усреднённой скоростью/направлением роя и локальной скоростью
// [LAB3][BOIDS] нормализуется и превращается в силу. Среднее здесь глобальное (через mip).
float3 Alignment( float3 vLocalPos, float3 vLocalVel, float3 vVel )
{
	float3 vDelta = normalize( vVel - vLocalVel );
	return (g_fMaxForce * vDelta);
}



// PS for particles
// [LAB3][BOIDS] ПОПАРНЫЕ СИЛЫ: этот pixel shader рассчитывает вклад каждого исходного boid
// [LAB3][BOIDS] для каждого принимающего texel. Оба варианта AccumulateForces используют его.
// [LAB3][BOIDS] Результаты PSForce складываются аппаратным AdditiveBlending в g_txForce.
float4 PSForce(PS_FORCE_IN input) : SV_Target
{   
	float3 texcoord = float3( input.tex, 0 );
	float3 vLocalPos = g_txParticleData.SampleLevel( g_samPoint, texcoord, 0 );
	texcoord.z = 1;
	float3 vLocalVel = g_txParticleData.SampleLevel( g_samPoint, texcoord, 0 );
	
	// Do we need to do anything?
	// [LAB3][BOIDS] input.pos — позиция агента-источника splat; vLocalPos — агент текущего texel.
	float3 vNeighborPos = input.pos;
	float3 delta = vLocalPos - vNeighborPos;
	float r2 = dot( delta, delta );
	float3 vTotalForce = float3(0,0,0);

	// [LAB3][BOIDS] Исключаем только совпадение позиций; отдельного радиуса восприятия нет.
	// [LAB3][BOIDS] Силы Separation и Avoid суммируются для этой пары, затем для всех пар.
	if( r2 > 0 )
	{
		vTotalForce = Separation( vLocalPos, vNeighborPos );
		vTotalForce += Avoid( vLocalPos, vLocalVel, vNeighborPos );
	}
	
	return float4(vTotalForce,1);
}



// Advance particles
// [LAB3][BOIDS] Технический VS для обновления: данные этой точки не участвуют в Boids-формулах.
VS_ADVANCE_OUT VSAdvance(VS_PARTICLE_IN input)
{
    VS_ADVANCE_OUT output;
    
    output.pos = float3(0,0,0);
    
    return output;
}


// Fullscreen GS for advancing particles
[maxvertexcount(12)]
// [LAB3][BOIDS] Генерирует три полноэкранных quad из одного входного point:
// [LAB3][BOIDS] irt/rtindex 0 = позиция, 1 = направление+скорость, 2 = up.
// [LAB3][BOIDS] PSAdvance запускается для каждого пикселя слоя, записывая новое состояние.
void GSAdvance(point VS_ADVANCE_OUT input[1], inout TriangleStream<GS_ADVANCE_OUT> SpriteStream)
{
	GS_ADVANCE_OUT output;
    
    // position quad
    output.irt = 0;
    output.rtindex = 0;
    for(int i=0; i<4; i++)
    {
        output.clippos = float4(g_positions[i],1);
        output.tex = g_texcoords[i];
        
        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
    
    // velocity quad
    output.irt = 1;
    output.rtindex = 1;
    for(int i=0; i<4; i++)
    {
        output.clippos = float4(g_positions[i],1);
        output.tex = g_texcoords[i];
        
        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
    
    // up quad
    output.irt = 2;
    output.rtindex = 2;
    for(int i=0; i<4; i++)
    {
        output.clippos = float4(g_positions[i],1);
        output.tex = g_texcoords[i];
        
        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
}


// PS for particles
// [LAB3][BOIDS] ГЛАВНЫЙ ШЕЙДЕР СИМУЛЯЦИИ BOIDS (один шаг на кадр).
// [LAB3][BOIDS] Читает старое состояние и суммарные силы, вычисляет управляющее воздействие,
// [LAB3][BOIDS] ограничивает силу/скорость и возвращает новое значение одного слоя данных.
float4 PSAdvance(PS_ADVANCE_IN input) : SV_Target
{   
	float3 texcoord = float3( input.tex, 0 );
	float3 pos = g_txParticleData.SampleLevel( g_samPoint, texcoord, 0 );

	// [LAB3][BOIDS] Старший mip слоя позиций — глобальное приближённое среднее всего роя;
	// [LAB3][BOIDS] оно затем используется в Cohesion(). Это не поиск соседей в радиусе.
	float3 avgPos = g_txParticleData.SampleLevel( g_samPoint, texcoord, g_iMaxMip );
	texcoord.z = 1;
	float4 dir = g_txParticleData.SampleLevel( g_samPoint, texcoord, 0 );

	// [LAB3][BOIDS] Усредняются закодированные direction.xyz и speed.w; произведение ниже
	// [LAB3][BOIDS] служит приближённой глобальной скоростью для Alignment().
	float4 avgVel = g_txParticleData.SampleLevel( g_samPoint, texcoord, g_iMaxMip );
	avgVel.xyz *= avgVel.w;
	texcoord.z = 2;
	float3 up = g_txParticleData.SampleLevel( g_samPoint, texcoord, 0 );

	
	// avoid force
	// [LAB3][BOIDS] Попарные Separation+Avoid уже накоплены в предыдущем проходе (PSForce).
	// [LAB3][BOIDS] g_fSeparationStrength масштабирует здесь ОБЕ составляющие этой суммы.
	float3 avoidforce = g_fSeparationStrength * g_txForce.SampleLevel( g_samPoint, input.tex, 0 );
	
	float3 vel = dir.xyz*dir.w;
	// [LAB3][BOIDS] ВТОРАЯ ЧАСТЬ Boids: независимые steering-силы от цели, центра и среднего
	// [LAB3][BOIDS] направления роя, а также управляемого шара. Каждая умножается на свой вес.
	float3 seekforce = g_fSeekStrength*Seek( pos, vel, g_vSeekPos );
	float3 cohesionforce = g_fCohesionStrength * Cohesion( pos, vel, avgPos );
	float3 alignmentforce = g_fAlignmentStrength * Alignment( pos, vel, avgVel.xyz );
	float3 fleeforce = g_fFleeStrength*Flee( pos, vel, g_vFleePos );
	
	// combine and clamp forces
	// [LAB3][BOIDS] ИТОГОВАЯ СИЛА: сумма всех влияний. Затем ограничение длины вектора MaxForce.
	// [LAB3][BOIDS] Именно это место удобно показывать преподавателю как ядро алгоритма Boids.
	float3 force = avoidforce + seekforce + fleeforce + cohesionforce + alignmentforce;
	if( dot(force,force) > g_fMaxForce*g_fMaxForce )
		force = normalize(force) * g_fMaxForce;
	
	float4 data = float4(0,0,0,0);
	float3 accel = force;	//assume mass of 1
	
	// update vel and clamp to max speed
	// [LAB3][BOIDS] Интегрирование скорости методом Эйлера: v(t+dt) = v(t) + force*dt.
	// [LAB3][BOIDS] Масса условно принята равной единице.
	float3 newVel = vel + accel * g_fElapsedTime;
	if( dot(newVel,newVel) > g_fMaxSpeed*g_fMaxSpeed )
		newVel = normalize(newVel) * g_fMaxSpeed;
		
	// [LAB3][BOIDS] Обновление позиции через СТАРУЮ скорость vel (а не newVel): p(t+dt) = p(t) + v(t)*dt.
	float3 newPos = pos + vel * g_fElapsedTime;
	
	// reorient ourselves
	float speed = length(newVel);
	float3 newDir = dir;
	if( speed > 0.001 )
		newDir = normalize( newVel );

	// [LAB3][BOIDS] Особенность оригинала: модуль записываемой скорости принудительно = MaxSpeed.
	// [LAB3][BOIDS] Следовательно, рассчитанное выше замедление по newVel не сохраняется в speed.
	speed = g_fMaxSpeed;	// perhaps allow for speed changes later
	float3 approxUp = normalize( up );
	float3 newRight = normalize( cross( approxUp, newDir ) );
	float3 newUp = normalize( cross( newDir, newRight ) );
	
	// [LAB3][BOIDS] Выбор выходного слоя по индексу, заданному GSAdvance:
	// [LAB3][BOIDS] 2 -> up, 1 -> direction.xyz + speed.w, 0 -> position.xyz.
	if( input.irt > 1 )
		data = float4(newUp,0); // up
	else if( input.irt > 0 )
		data = float4(newDir,speed); // velocity
	else
		data = float4(newPos,0); // position
	
	return data;
}


// boids
// [LAB3][BOIDS] ВИЗУАЛИЗАЦИЯ: vertex shader одного экземпляра модели.
// [LAB3][BOIDS] По SV_InstanceID берёт текущие pos/dir/up и строит локальную ориентацию;
// [LAB3][BOIDS] вся динамика была рассчитана ранее в PSAdvance().
VS_BOID_OUT VSBoids( VS_BOID_IN input )
{
	VS_BOID_OUT output;
	
	// Lookup the particle position
    uint id = input.instanceID;
    int4 texcoord;
    texcoord.x = id % g_iTexSize;
    texcoord.y = id / g_iTexSize;
    texcoord.z = 0; // position
    texcoord.w = 0; // mip level

    float3 pos = g_txParticleData.Load(texcoord).xyz;
    texcoord.z = 1;
    float3 dir = g_txParticleData.Load(texcoord).xyz;
    texcoord.z = 2;
    float3 up = g_txParticleData.Load(texcoord).xyz;
    float3 right = normalize( cross( up, dir ) );
    
    float3x3 rotMatrix = { right*g_fBoidScale, up*g_fBoidScale, dir*g_fBoidScale };
   
	output.norm = mul( input.norm, rotMatrix );
	float4 transpos;
	transpos.xyz = mul( input.pos, (float3x3)rotMatrix );
	transpos.xyz += pos;
	transpos.w = 1;
	
	output.pos = mul( transpos, g_mWorldViewProj );
	
	return output;
}


// [LAB3][BOIDS] Простейшее освещение/окраска boid-модели; правил поведения здесь нет.
float4 PSBoids( PS_BOID_IN input ) : SV_TARGET
{
	float3 norm = normalize( input.norm );
	return float4(1,0.0,0.0,1)*max( 0.3, saturate(dot(norm,g_vLightPos)) );
}


// mesh
VS_MESH_OUT VSMesh( VS_MESH_IN input )
{
	VS_MESH_OUT output;
	
	output.pos = mul( float4(input.pos,1), g_mWorldViewProj );
	output.norm = mul( input.norm, (float3x3)g_mWorld );
	output.tex = input.tex;
	
	return output;
}


float4 PSMesh( PS_MESH_IN input ) : SV_TARGET
{
	float4 diffuse = g_txDiffuse.Sample( g_samLinear, input.tex );
	float3 norm = normalize( input.norm );
	return diffuse*saturate(dot(norm,g_vLightPos));
}


// RenderParticles - renders particles on the screen
technique11 RenderParticles
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VSParticleDraw() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSParticleDraw() ) );
        SetPixelShader( CompileShader( ps_4_0, PSParticleDraw() ) );
        
        SetBlendState( ParticleBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( DisableDepth, 0 );
    }  
}



// AccumulateForcesGS - accumulates forces between particles using the GS to create screen-aligned quads
// [LAB3][BOIDS] GPU ПРОХОД 2A: VSForceGS -> GSForce -> PSForce; additive blending суммирует силы.
technique11 AccumulateForcesGS
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VSForceGS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSForce() ) );
        SetPixelShader( CompileShader( ps_4_0, PSForce() ) );
        
        SetBlendState( AdditiveBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( DisableDepth, 0 );
    }  
}


// AccumulateForces - accumulates forces between particles using premade screen-aligned quads
// [LAB3][BOIDS] GPU ПРОХОД 2B: VSForce -> PSForce без GS, с заранее созданными splat-треугольниками.
technique11 AccumulateForces
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VSForce() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSForce() ) );
        
        SetBlendState( AdditiveBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( DisableDepth, 0 );
    }  
}


// AdvanceParticles - advances particles based on force, vel, pos
// [LAB3][BOIDS] GPU ПРОХОД 3: VSAdvance -> GSAdvance -> PSAdvance; пишет все 3 слоя нового состояния.
technique11 AdvanceParticles
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VSAdvance() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSAdvance() ) );
        SetPixelShader( CompileShader( ps_4_0, PSAdvance() ) );
        
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( DisableDepth, 0 );
    }  
}


// RenderBoids - renders boids
// [LAB3][BOIDS] GPU ПРОХОД 5: инстансированная визуализация, VSBoids использует SV_InstanceID.
technique11 RenderBoids
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VSBoids() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSBoids() ) );
        
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( EnableDepth, 0 );
    }  
}


// RenderMesh 
technique11 RenderMesh
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VSMesh() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSMesh() ) );
        
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( EnableDepth, 0 );
    }  
}