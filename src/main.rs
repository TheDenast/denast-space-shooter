use macroquad::prelude::*;

// Constants for game settings
const PLAYER_SPEED: f32 = 300.0;
const PLAYER_SIZE: f32 = 0.8; // Scale factor for the player sprite

struct Player {
    texture: Texture2D,
    position: Vec2,
}

impl Player {
    async fn new() -> Self {
        let texture = load_texture("assets/sprites/player.png").await.unwrap();
        let texture_height = texture.height();
        
        Self {
            texture,
            position: Vec2::new(
                screen_width() / 2.0,
                screen_height() - texture_height * PLAYER_SIZE - 10.0,
            ),
        }
    }

    fn update(&mut self, dt: f32) {
        // Horizontal movement
        if is_key_down(KeyCode::Left) || is_key_down(KeyCode::A) {
            self.position.x -= PLAYER_SPEED * dt;
        }
        if is_key_down(KeyCode::Right) || is_key_down(KeyCode::D) {
            self.position.x += PLAYER_SPEED * dt;
        }

        // Vertical movement
        if is_key_down(KeyCode::Up) || is_key_down(KeyCode::W) {
            self.position.y -= PLAYER_SPEED * dt;
        }
        if is_key_down(KeyCode::Down) || is_key_down(KeyCode::S) {
            self.position.y += PLAYER_SPEED * dt;
        }

        // Keep player within screen bounds
        self.position.x = self.position.x.clamp(
            0.0,
            screen_width() - self.texture.width() * PLAYER_SIZE,
        );
        self.position.y = self.position.y.clamp(
            0.0,
            screen_height() - self.texture.height() * PLAYER_SIZE,
        );
    }

    fn draw(&self) {
        draw_texture_ex(
            &self.texture,
            self.position.x,
            self.position.y,
            WHITE,
            DrawTextureParams {
                dest_size: Some(Vec2::new(
                    self.texture.width() * PLAYER_SIZE,
                    self.texture.height() * PLAYER_SIZE,
                )),
                ..Default::default()
            },
        );
    }
}

#[macroquad::main("Space Shooter")]
async fn main() {
    // Load background texture
    let background = load_texture("assets/sprites/spacebg.png").await.unwrap();
    let mut player = Player::new().await;

    loop {
        // Get delta time for smooth movement
        let dt = get_frame_time();

        // Update
        player.update(dt);

        // Draw
        clear_background(BLACK);
        
        // Draw background (tiled if needed)
        draw_texture_ex(
            &background,
            0.0,
            0.0,
            WHITE,
            DrawTextureParams {
                dest_size: Some(Vec2::new(screen_width(), screen_height())),
                ..Default::default()
            },
        );

        // Draw player
        player.draw();

        next_frame().await;
    }
}
