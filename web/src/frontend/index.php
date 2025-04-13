<?php
/**
 * Main entry point for rendering the IoT Data Dashboard using Twig.
 * Dynamically loads environment-specific asset paths (Vite dev or production build).
 *
 * @author Yanis Deplazes
 * @date 2025-04-09
 */

require_once __DIR__ . '/vendor/autoload.php';

use Twig\Environment;
use Twig\Loader\FilesystemLoader;

$loader = new FilesystemLoader(__DIR__ . '/templates');
$twig = new Environment($loader);

// Detect environment
$isDev = ($_SERVER['SERVER_NAME'] ?? '') === 'dev.localhost';
$jsScript = '';
$cssLink = '';

if ($isDev) {
    $jsScript = 'http://localhost:5173/assets/js/main.js';
} else {
    // Load from manifest
    $manifestPath = __DIR__ . '/../../build/frontend/assets/manifest.json';
    $entry = 'assets/js/main.js';

    if (file_exists($manifestPath)) {
        $manifest = json_decode(file_get_contents($manifestPath), true);
        if (isset($manifest[$entry]['file'])) {
            $jsScript = '/assets/' . $manifest[$entry]['file'];
        }
        if (isset($manifest[$entry]['css'][0])) {
            $cssLink = '/assets/' . $manifest[$entry]['css'][0];
        }
    } else {
        $jsScript = '/assets/main.js'; // fallback
    }
}

echo $twig->render('index.twig', [
    'title' => 'IoT Data Dashboard',
    'project_name' => 'ATMOS',
    'settings_label' => 'Settings',
    'author_name' => 'Yanis Deplazes',
    'author_link' => 'https://yanis.io',
    'year' => '2025',
    'is_dev' => $isDev,
    'js_script' => $jsScript,
    'css_link' => $cssLink,
]);
