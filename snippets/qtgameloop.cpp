// This is a demonstration of running a traditional game loop inside of
// Qt

int main( int argc, char* argv[] )
{
    // init stuff like you normally would for a Qt app
    QApplication app;

    //
    // main game loop
    //
    QTime currentTime = QTime::currentTime();
    QTime lastTime    = QTime::currentTime();

    while ( game.isRunning() )
    {
        // Process queued windowing events
        app.processEvents();

        // Calculate tick delta
        currentTime = QTime::currentTime();
        int deltaT  = lastTime.msecsTo( currentTime );

        // Run the game simulation for as many time steps as required
        // to catch up
        while ( deltaTime >= SIMULATE_TIME_STEP )
        {
            game.simulate();

            deltaT   -= SIMULATE_TIME_STEP;
            lastTime  = lastUpdate.addMSecs( SIMULATE_TIME_STEP );
        }

        // Now render the game
        renderer.setInterpolateAmt( static_cast<float>(deltaT) /
                                    static_cast<float>(SIMULATE_TIME_STEP) );
        renderer.renderWorld();
        renderer.renderGUI();
        renderer.commit();
    }

    app.exit();
}
