//
//  Particlewall.cpp
//  ParticleWall
//
//  Created by Mike Allison on 6/27/13.
//
//

#include "Particlewall.h"

Particlewall::Particlewall(){
    
}

void Particlewall::loadMovie(){
    
    try{
        video = qtime::MovieGl( loadResource( "MVI_3460.MOV" ) );
        video.setLoop();
        video.play();
    }
    catch ( ... ) {
        console() << "Uable to load movie" << endl;
        video.reset();
    }
    
    video.setVolume(0.f);
}

void Particlewall::setup(){
    
    threshold = .3f;
    iterThresh = 127;
    mWTFoffset = 193.;
    mCC1 = 0;
    mCC2 = 0;
    mCC3 = 0;
    debug = true;
    glEnable(GL_DEPTH_TEST);
    
    try {
		drawShader = gl::GlslProg::create( loadResource( RES_DRAW_VERT ), loadResource( RES_DRAW_FRAG ) );
	}
	catch( gl::GlslProgCompileExc &exc ) {
		cout << "Draw Shader compile error: " << endl;
		cout << exc.what();
	}
	catch( gl::GlslNullProgramExc ex ) {
        console() << "File not found: " << ex.what() << endl;
    }
    
    try {
		updateShader = gl::GlslProg::create( loadResource( RES_UPDATE_PASS ), loadResource( RES_UPDATE_FRAG ) );
	}
	catch( gl::GlslProgCompileExc &exc ) {
		std::cout << "update Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	catch( gl::GlslNullProgramExc ex ) {
        console() << "File not found: " << ex.what() << endl;
	}

    
    img = Surface32f(FBO_WIDTH, FBO_WIDTH, false);
    Surface32f::Iter iter = img.getIter(img.getBounds());
    while(iter.line()){
        while(iter.pixel()){
            
            img.setPixel(iter.getPos(), ColorAf(((float)iter.getPos().x / (float)img.getWidth()),
                                                ((float)iter.getPos().y / (float)img.getHeight()),
                                                .5f,
                                                .5f));
            
        }
    }
    
    tex = gl::Texture( img );
    
    createVbo();
    
    loadMovie();
    
    kinect.setup();
        
    gl::Fbo::Format format, defaultFormat;
    
    defaultFormat.setSamples( 4 );
    defaultFormat.enableColorBuffer();
    defaultFormat.enableDepthBuffer();
    defaultFormat.setColorInternalFormat(GL_RGBA32F_ARB);
    
    format.setSamples( 4 );
    format.enableColorBuffer();
    format.enableDepthBuffer();
    format.enableMipmapping();
    //format.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
    //format.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
    format.setMagFilter(GL_NEAREST_MIPMAP_NEAREST);
    format.setMinFilter(GL_NEAREST_MIPMAP_NEAREST);
    format.setColorInternalFormat(GL_RGBA32F_ARB);
    
    ping = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format );
    pong = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format );
    kinectFbo = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, defaultFormat );
    videoFbo = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, defaultFormat );
    render = gl::Fbo(getWindowWidth(), getWindowHeight(), format);
    
    reset();
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    cam.setPerspective(60.f, getWindowAspectRatio(), 1.f, 1000.f);
    cam.lookAt(Vec3f(getWindowWidth()/2,getWindowHeight()/2,500.f), Vec3f(getWindowWidth()/2,getWindowHeight()/2, 0.f), Vec3f::yAxis());
    
    
}

void Particlewall::createVbo(){
    
    int totalVerts = VERTICES_X * VERTICES_Y;
    //int totalQuads = ( VERTICES_X -1 ) * ( VERTICES_Y - 1 );
    gl::VboMesh::Layout layout;
    layout.setStaticIndices();
    layout.setStaticPositions();
    layout.setStaticTexCoords2d();
    layout.setStaticNormals();
    mesh = gl::VboMesh::create( totalVerts, totalVerts, layout, GL_POINTS);
    glPointSize(1.0f);
    vector<uint32_t> indicies;
    vector<Vec2f> texCoords;
    vector<Vec3f> positions;
    for(int x = 0; x < VERTICES_X; x++){
        for(int y = 0; y < VERTICES_Y ; y++){
            positions.push_back( Vec3f( x, y, 0.f ) );
            indicies.push_back( x + VERTICES_X * y );
            texCoords.push_back( Vec2f( (getWindowWidth() - x) / (float)VERTICES_X, (getWindowHeight() - y) / (float)VERTICES_Y ) );
        }
    }
    mesh->bufferPositions( positions );
    mesh->bufferIndices( indicies );
    mesh->bufferTexCoords2d( 0, texCoords );
    
    mesh->unbindBuffers();
    
    indicies.clear();
    texCoords.clear();
    positions.clear();
}

void Particlewall::update(){
    
    kinect.update();
    
    gl::Texture kinectImage;
    Surface kinectPix;
    
    if(kinect.getTex()){
        kinectImage = kinect.getTex();
    }
    
    if(kinect.getSurface()){
        kinectPix = kinect.getSurface();
    }
    
    
    Surface::Iter it = kinectPix.getIter(kinectPix.getBounds());
    
    int count_1 = 1;
    int averageX_1 = 0;
    int averageY_1 = 0;
    int avgColor_1 = 0;
    int count_2 = 1;
    int averageX_2 = 0;
    int averageY_2 = 0;
    int avgColor_2 = 0;
    int count_3 = 1;
    int averageX_3 = 0;
    int averageY_3 = 0;
    int avgColor_3 = 0;
    int count_total = 1;
    int averageX_total = 0;
    int averageY_total = 0;
    int avgColor_total = 0;
    
    while (it.line()) {
        while (it.pixel()){
            if(it.r() > iterThresh && it.x() < (it.getWidth()/3) ){
                avgColor_1 += it.r();
                averageX_1 += it.x();
                averageY_1 += it.y();
                count_1++;
            }
            if(it.r() > iterThresh && it.x() < (it.getWidth()/3)*2 && it.x() > (it.getWidth()/3) ){
                avgColor_2 += it.r();
                averageX_2 += it.x();
                averageY_2 += it.y();
                count_2++;
            }
            if(it.r() > iterThresh && it.x() < it.getWidth() && it.x() > (it.getWidth()/3)*2 ){
                avgColor_3 += it.r();
                averageX_3 += it.x();
                averageY_3 += it.y();
                count_3++;
            }
            
        }
    }
    
    
    
    mCC1 = (int)lmap((float)avgColor_1/count_1, 0.f, 255.f, 0.f, 127.f);
    mCC2 = (int)lmap((float)avgColor_2/count_2, 0.f, 255.f, 0.f, 127.f);
    mCC3 = (int)lmap((float)avgColor_3/count_3, 0.f, 255.f, 0.f, 127.f);
    
    cout<< mCC1 << " " << mCC2 << " " << mCC3 <<endl;

    avgColor_total = avgColor_1 + avgColor_2 + avgColor_3;
    averageX_total = averageX_1 + averageX_2 + averageX_3;
    averageY_total = averageY_1 + averageY_2 + averageY_3;
    count_total = count_1 + count_2 + count_3;
    
    Rectf rect = Rectf( Vec2i(0,0), Vec2i(FBO_WIDTH,FBO_HEIGHT) );
    
    kinectFbo.bindFramebuffer();
    
    gl::setMatricesWindow( kinectFbo.getSize(), false ); // false to prevent vertical flipping
    gl::setViewport(kinectFbo.getBounds());
    if(kinect.getTex()){
        gl::clear();
        gl::draw( kinectImage,  Rectf(Vec2f(0,0),Vec2f(FBO_WIDTH,FBO_HEIGHT)) );
    }
    
    kinectFbo.unbindFramebuffer();
    
    bool usePing = getElapsedFrames() % 2 == 0;
    
    gl::Fbo &toFbo = usePing ? ping : pong;
    gl::Fbo &fromFbo = usePing ? pong : ping;
    
    
    toFbo.bindFramebuffer();
    
    gl::setMatricesWindow( toFbo.getSize(), false ); // false to prevent vertical flipping
    gl::setViewport(toFbo.getBounds());
    gl::clear();
    
    fromFbo.bindTexture(0);
    kinectFbo.bindTexture(1);
  //  kinect.getTex().bind(1);
    tex.bind(2);
    
    updateShader->bind();
    updateShader->uniform("tex", 0);
    updateShader->uniform("displacement", 1);
    updateShader->uniform("startTex", 2);
    updateShader->uniform("threshold", threshold);
    gl::draw(fromFbo.getTexture());
    updateShader->unbind();
    
    tex.unbind();
    kinectFbo.unbindTexture();
   // kinect.getTex().unbind();
    fromFbo.unbindTexture();
    toFbo.unbindFramebuffer();
    
    videoFbo.bindFramebuffer();
    gl::setMatricesWindow( videoFbo.getSize(), false ); // false to prevent vertical flipping
    gl::setViewport(videoFbo.getBounds());
    if(video){
        gl::clear();
        gl::draw( video.getTexture() , rect );
    }
    videoFbo.unbindFramebuffer();
    
    render.bindFramebuffer();
    gl::clear();
    gl::setMatrices( cam );
    gl::translate(0.,0.,-mWTFoffset);
    gl::scale(Vec3f((float)getWindowWidth(),(float)getWindowHeight(),1.f));
    gl::setViewport(getWindowBounds());
    
    gl::Texture pingpong = usePing ? ping.getTexture() : pong.getTexture();
    
    pingpong.bind(0);
    gl::Texture & v = videoFbo.getTexture();
    v.bind(1);
    
    drawShader->bind();
    drawShader->uniform("tex", 0);
    drawShader->uniform("vid", 1);
    
    gl::draw( mesh );
    
    drawShader->unbind();
    
    v.unbind();
    pingpong.unbind();
    render.unbindFramebuffer();
    
}

void Particlewall::draw(){
    

    if(!debug){
        gl::Texture renderedScene = render.getTexture();
        renderedScene.setFlipped();
        
        gl::setMatricesWindow(getWindowSize());
        gl::setViewport(getWindowBounds());
        gl::draw( renderedScene );
        
    }
    else {
        gl::setMatricesWindow(getWindowSize());
        gl::setViewport(getWindowBounds());
        kinect.draw();
    }
    
}

void Particlewall::reset(){
    
    
    ping.bindFramebuffer();
    
    gl::setMatricesWindow( ping.getSize(), false ); // false to prevent vertical flipping
    
    gl::clear();
    gl::draw( tex );
    ping.unbindFramebuffer();
    
    pong.bindFramebuffer();
    
    gl::setMatricesWindow( pong.getSize(), false ); // false to prevent vertical flipping
    
    gl::clear();
    gl::draw( tex );
    pong.unbindFramebuffer();
    
}

void Particlewall::setDebug(){
    debug = !debug;
}

void Particlewall::thresholdUP(){
    threshold+=.1f;
    //  cam.setEyePoint( Vec3f(camX, camY, ++camZ) );
    
}

void Particlewall::thresholdDOWN(){
    threshold-=.1f;
    //  cam.setEyePoint( Vec3f(camX, camY, --camZ) );
    
}
