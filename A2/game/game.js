import * as THREE from 'three';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader';
// import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls';

let started = false, end = false, countdown = 3;
let scene, camera, renderer, camera_position1, cameraDirection, topview;
let Speed = 0, rotateSpeed = 0, stats, checkpointz = 1, checkpointx = 1;
let mcqueen, mcqueenDirection, carSpeed, maxSpeed, lasthit = [new Date, new Date, new Date, new Date];
let car1, car2, car3, car4, currangle = [0, 0, 0, 0], radius = [0, 0, 0, 0], healthCars = [100, 100, 100, 100];
let indexradiuschange2 = [[255, 285], [255, 270], [270, 285], [285, 270]], indexradiuschange1 = [[225, 270], [225, 240], [250, 225], [240, 225]], indexradiuschange2d = 1;
let health = 0, score = 0, distance_travelled = 0, fuel = 0, start_time, timeCars = [0, 0, 0, 0], checkpointsz = [0, 0], checkpointsx = [0, 0];
let fuels_pos = [[], []], fuelpoints = [0, 0], tank1, tank2, lastfuel = [new Date, new Date];
const keys = { w: false, s: false, a: false, d: false, c: true };
let max_laps = 3;

const loader = new GLTFLoader();

function init() {
    scene = new THREE.Scene();
    scene.background = new THREE.Color(0x87CEEB);

    camera = new THREE.PerspectiveCamera(
        75,
        window.innerWidth / window.innerHeight,
        0.1,
        1000
    );

    topview = new THREE.PerspectiveCamera(
        90,
        window.innerWidth / window.innerHeight,
        0.1,
        1000
    );

    // const axesHelper = new THREE.AxesHelper(2);
    // scene.add(axesHelper);

    stats = createStats();
    document.body.appendChild(stats.domElement);

    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    const light = new THREE.DirectionalLight(0xFFC0CB);
    light.position.set(0, 50, 150);
    const ambient = new THREE.AmbientLight(0x707070);

    // scene.add(camera);
    scene.add(light);
    scene.add(ambient);

    const geometry = new THREE.BoxGeometry(4, 4, 4);
    // const material = new THREE.MeshNormalMaterial();
    let material = new THREE.MeshPhongMaterial({ color: 0xff3632 });
    // const material = new THREE.MeshBasicMaterial({ color: 0x0000ff });

    const texture = new THREE.TextureLoader().load('textures/crate.gif');
    // const material = new THREE.MeshPhongMaterial({ map: texture });
    tank1 = new THREE.Mesh(geometry, material);
    scene.add(tank1);
    tank2 = new THREE.Mesh(geometry, material);
    scene.add(tank2);

    const center = new THREE.Mesh(new THREE.SphereGeometry(0, 0, 0));
    scene.add(center);
    center.add(camera);

    tank1.position.set(250, 2.5, 0);

    // init_plane();
    init_track();
    init_mcqueen();
    init_cars();
    init_people();
    init_fuel();

    // camera.position.z = -2;
    camera.position.y = 10;
    camera.position.x = 270;
    camera.position.z = -20;
    cameraDirection = new THREE.Vector3(0, 0, 0);
    camera.rotation.y = - Math.PI;
    camera_position1 = new THREE.Vector3(camera.position.x, camera.position.y, camera.position.z);

    topview.position.set(0, 80, 0);
    topview.lookAt(0, 0, 0);
    camera.add(topview);
}

function init_track() {
    const geometry = new THREE.RingGeometry(210, 300, 400);
    const material = new THREE.MeshPhongMaterial({ color: 0x000000, side: THREE.DoubleSide });
    const track = new THREE.Mesh(geometry, material);

    track.rotation.x = Math.PI / 2;
    track.position.x = 0;
    track.position.y = 0;
    track.position.z = 0.01;
    scene.add(track);

    const points = [];
    points.push(new THREE.Vector3(210, 0.11, 0));
    points.push(new THREE.Vector3(300, 0.11, 0));

    const geometry_line = new THREE.BufferGeometry().setFromPoints(points);
    const material_line = new THREE.LineDashedMaterial({ color: 0xffffff, linewidth: 20, scale: 1, dashSize: 10, gapSize: 5, });
    const line = new THREE.Line(geometry_line, material_line);
    scene.add(line);

    let pts = new THREE.Path().absarc(0, 0, 255, 0, Math.PI * 2).getPoints(200);
    let ring_geometry = new THREE.BufferGeometry().setFromPoints(pts);
    ring_geometry.rotateX(Math.PI * 0.5);
    let ring_material = new THREE.LineDashedMaterial({ color: 0xffffff, linewidth: 1, scale: 1, dashSize: 3, gapSize: 3, });
    let ring = new THREE.Line(ring_geometry, ring_material);
    ring.computeLineDistances();
    scene.add(ring)

    const geometry1 = new THREE.RingGeometry(180, 315, 400);
    const material1 = new THREE.MeshPhongMaterial({ color: 0x136d15, side: THREE.DoubleSide });
    const grass = new THREE.Mesh(geometry1, material1);

    grass.rotation.x = Math.PI / 2;
    grass.position.x = 0;
    grass.position.y = -0.1;
    grass.position.z = 0.01;
    scene.add(grass);

    const geometry2 = new THREE.RingGeometry(1, 190, 400);
    // const texture = new THREE.TextureLoader().load('textures/grass.png');
    // const material1 = new THREE.MeshPhongMaterial({ map: texture });
    const material2 = new THREE.MeshPhongMaterial({ color: 0xc2b280, side: THREE.DoubleSide });
    const sand = new THREE.Mesh(geometry2, material2);

    sand.rotation.x = Math.PI / 2;
    sand.position.x = 0;
    sand.position.y = -0.2;
    sand.position.z = 0.01;
    scene.add(sand);


    loader.load(
        './models/circle/scene.gltf',
        function (gltf) {
            let scale = 26;
            gltf.scene.scale.set(scale, 1, scale);
            gltf.scene.translateY(0.5);
            // gltf.scene.rotateY(-Math.PI / 2)
            scene.add(gltf.scene);
        },
        function (xhr) {
            console.log((xhr.loaded / xhr.total) * 100 + '% loaded');
        },
        function (error) {
            console.error(error);
        }
    );

    loader.load(
        './models/circle/scene.gltf',
        function (gltf) {
            let scale = 40;
            gltf.scene.scale.set(scale, 1, scale);
            gltf.scene.translateY(0.5);
            // gltf.scene.rotateY(-Math.PI / 2)
            scene.add(gltf.scene);
        },
        function (xhr) {
            console.log((xhr.loaded / xhr.total) * 100 + '% loaded');
        },
        function (error) {
            console.error(error);
        }
    );
}

function init_mcqueen() {
    mcqueen = new THREE.Object3D();
    loader.load(
        './models/mcqueen/scene.gltf',
        function (gltf) {
            mcqueen = gltf.scene;
            gltf.scene.translateX(270);
            gltf.scene.translateY(0.1);
            scene.add(gltf.scene);
        },
        function (xhr) {
            console.log((xhr.loaded / xhr.total) * 100 + '% loaded');
        },
        function (error) {
            console.error(error);
        }
    );

    mcqueenDirection = new THREE.Vector3(0, 0, 0);

    carSpeed = 0;
    maxSpeed = 3;

    health = 100;
    fuel = 100;
    score = 0;
    distance_travelled = 0;
}

function createWheels() {
    const geometry = new THREE.BoxGeometry(12, 12, 33);
    const material = new THREE.MeshLambertMaterial({ color: 0x333333 });
    const wheel = new THREE.Mesh(geometry, material);
    return wheel;
}
function createCar(colour, position) {
    // From freecodecamp
    const car = new THREE.Group();

    const backWheel = createWheels();
    backWheel.position.y = 6;
    backWheel.position.x = -18;
    car.add(backWheel);

    const frontWheel = createWheels();
    frontWheel.position.y = 6;
    frontWheel.position.x = 18;
    car.add(frontWheel);

    const main = new THREE.Mesh(
        new THREE.BoxGeometry(60, 15, 30),
        new THREE.MeshLambertMaterial({ color: colour })
    );
    main.position.y = 12;
    car.add(main);

    const cabin = new THREE.Mesh(
        new THREE.BoxGeometry(33, 12, 24),
        new THREE.MeshLambertMaterial({ color: 0xffffff })
    );
    cabin.position.x = -6;
    cabin.position.y = 25.5;
    car.add(cabin);

    car.scale.multiplyScalar(0.15);
    car.position.set(position.x, position.y, position.z);
    car.rotateY(-Math.PI / 2);

    return car;
}

function init_cars() {
    radius[0] = 225;
    radius[1] = 240;
    radius[2] = 255;
    radius[3] = 285;

    car1 = createCar(0x523ddb, new THREE.Vector3(225, 1, 0));
    car2 = createCar(0xd62bd9, new THREE.Vector3(240, 0.1, 0));
    car3 = createCar(0x0fbd26, new THREE.Vector3(255, 0.1, 0));
    car4 = createCar(0xc6e334, new THREE.Vector3(285, 0.1, 0));

    scene.add(car1);
    scene.add(car2);
    scene.add(car3);
    scene.add(car4);

    for (let i = 0; i < 4; i += 1)
        timeCars[i] = max_laps * (2 * Math.PI / (((i + 1) * Math.PI / 1260) * 60));
}

function init_people() {
    loader.load(
        './models/people/scene.gltf',
        function (gltf) {
            gltf.scene.scale.set(0.2, 0.2, 0.2);
            gltf.scene.translateX(304);
            gltf.scene.rotateY(-Math.PI / 2)
            scene.add(gltf.scene);
        },
        function (xhr) {
            console.log((xhr.loaded / xhr.total) * 100 + '% loaded');
        },
        function (error) {
            console.error(error);
        }
    );
    loader.load(
        './models/people/scene.gltf',
        function (gltf) {
            gltf.scene.scale.set(0.2, 0.2, 0.2);
            gltf.scene.translateX(-304);
            gltf.scene.rotateY(Math.PI / 2)
            scene.add(gltf.scene);
        },
        function (xhr) {
            console.log((xhr.loaded / xhr.total) * 100 + '% loaded');
        },
        function (error) {
            console.error(error);
        }
    );
    loader.load(
        './models/people/scene.gltf',
        function (gltf) {
            gltf.scene.scale.set(0.2, 0.2, 0.2);
            gltf.scene.translateZ(-304);
            scene.add(gltf.scene);
        },
        function (xhr) {
            console.log((xhr.loaded / xhr.total) * 100 + '% loaded');
        },
        function (error) {
            console.error(error);
        }
    );
    loader.load(
        './models/people/scene.gltf',
        function (gltf) {
            gltf.scene.scale.set(0.2, 0.2, 0.2);
            gltf.scene.translateZ(304);
            gltf.scene.rotateY(-Math.PI);
            scene.add(gltf.scene);
        },
        function (xhr) {
            console.log((xhr.loaded / xhr.total) * 100 + '% loaded');
        },
        function (error) {
            console.error(error);
        }
    );
    loader.load(
        './models/crowd/scene.gltf',
        function (gltf) {
            let scale = 0.75;
            gltf.scene.scale.set(scale, scale, scale);
            // gltf.scene.translateX(300);
            gltf.scene.rotateY(-Math.PI / 2)
            scene.add(gltf.scene);
        },
        function (xhr) {
            console.log((xhr.loaded / xhr.total) * 100 + '% loaded');
        },
        function (error) {
            console.error(error);
        }
    );
}

function init_fuel() {
    let i = 0;
    for (let j = 0; j < max_laps; j += 1) {
        let z = Math.pow(-1, i) * randomGenerator(215, 290);
        fuels_pos[i].push(new THREE.Vector3(0, 2.5, z));
    }
    tank1.position.set(fuels_pos[i][0].x, fuels_pos[i][0].y, fuels_pos[i][0].z);
    i = 1;
    for (let j = 0; j < max_laps; j += 1) {
        let z = Math.pow(-1, i) * randomGenerator(215, 290);
        fuels_pos[i].push(new THREE.Vector3(0, 2.5, z));
    }
    tank2.position.set(fuels_pos[i][0].x, fuels_pos[i][0].y, fuels_pos[i][0].z);
}

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();

    renderer.setSize(window.innerWidth, window.innerHeight);
}

function randomGenerator(min, max) {
    return Math.floor(Math.random() * (max - min)) + min;
}

function getTime() {
    let currtime = new Date;
    return (currtime - start_time) / 1000 - 3;
}

function convert_min_sec() {
    let sec = getTime()
    return Math.floor(sec / 60).toString() + ":" + Math.floor(sec - 60 * Math.floor(sec / 60)).toString();
}

function countdown_fn() {
    countdown = Math.ceil(2 - getTime());
    if (countdown > 0)
        document.getElementById('end').innerHTML = "<h1>" + countdown.toString() + "</h1><br/>";
    else
        document.getElementById('end').innerHTML = "";
}

function nextfuelcan() {
    let dist = -1;
    if (mcqueen.position.x > 0 && fuelpoints[0] < max_laps)
        return Math.round(100 * Math.sqrt(Math.pow(mcqueen.position.x - tank1.position.x, 2) + Math.pow(mcqueen.position.z - tank1.position.z, 2))) / 100;
    else if (mcqueen.position.x < 0 && fuelpoints[1] < max_laps)
        return Math.round(100 * Math.sqrt(Math.pow(mcqueen.position.x - tank2.position.x, 2) + Math.pow(mcqueen.position.z - tank2.position.z, 2))) / 100;
    return -1;
}

function updateStats() {
    document.getElementById('fuel').innerHTML = Math.ceil(Math.max(0, fuel)).toString();
    document.getElementById('speed').innerHTML = Math.trunc((fuel > 0 && health > 0 && !end && countdown == 0) ? carSpeed * 100 : 0).toString();
    document.getElementById('health').innerHTML = Math.ceil(Math.max(0, health)).toString();
    document.getElementById('score').innerHTML = Math.ceil(score).toString();

    if (!end && countdown == 0) {
        if (fuel > 0 && health > 0) {
            let time = getTime(), mileage = 0;
            document.getElementById('time').innerHTML = convert_min_sec();
            if (fuel < 100)
                mileage = distance_travelled / (100 - fuel);
            document.getElementById('mileage').innerHTML = "Mileage: " + (Math.round(mileage * 100) / 100).toString();
            let next_fuel = nextfuelcan();
            document.getElementById('next').innerHTML = "Next Fuel Can: " + (next != -1 ? next_fuel.toString() : "XX");

            if (time > timeCars[0]) {
                end = true;
                document.getElementById('end').innerHTML = "<h1>GAME OVER!!</h1><br/><h2>RANK: LAST</h2>";
            }
            if (checkpointsz[0] >= max_laps && checkpointsz[1] >= max_laps) {
                if (checkpointsx[0] >= max_laps && checkpointsx[1] >= max_laps) {
                    end = true;
                    let rank = 4;
                    if (time < timeCars[3])
                        rank = 1;
                    else if (time < timeCars[2])
                        rank = 2;
                    else if (time < timeCars[1])
                        rank = 3;
                    else
                        rank = 4;
                    document.getElementById('end').innerHTML = "<h1>GAME OVER!!</h1><br/><h2>RANK: " + rank.toString() + "</h2>";
                }
            }
        }
        else {
            let msg = "";
            if (fuel < 0)
                msg = "Out of Fuel";
            else
                msg = "Car Crashed; Zero Health"
            msg = "<h1>GAME OVER!</h1><br/><h1>" + msg + "</h1>";
            msg += "<br/><h2>RANK: LAST</h2>"
            document.getElementById('end').innerHTML = msg;
        }
    }
}

function checkCollectFuel(tank, index) {
    if (Date.now() < lastfuel[index])
        return -1;
    let dist = Math.sqrt(Math.pow(mcqueen.position.x - tank.position.x, 2) + Math.pow(mcqueen.position.z - tank.position.z, 2));
    if (dist < 5) {
        fuel = Math.min(100, fuel + 25);

        fuelpoints[index] += 1;
        if (fuelpoints[index] >= max_laps)
            scene.remove(tank);
        else {
            let positioning = fuels_pos[index][fuelpoints[index]]
            tank.position.set(positioning.x, positioning.y, positioning.z);
        }

        lastfuel[index] = new Date();
        lastfuel[index].setSeconds(lastfuel[index].getSeconds() + 3);
    }

    return 0;
}

function updateFuel() {
    checkCollectFuel(tank1, 0);
    checkCollectFuel(tank2, 1);
}

function checkCollision(car, index) {
    if (Date.now() < lasthit[index] || healthCars[index] < 10)
        return -1;
    let dist = Math.sqrt(Math.pow(mcqueen.position.x - car.position.x, 2) + Math.pow(mcqueen.position.z - car.position.z, 2));
    if (dist < 8) {
        healthCars[index] -= ((8 - dist) * 10 + 15);
        health -= ((8 - dist) * 10 + 10);

        score -= (100 - health);

        if (healthCars[index] < 10) {
            scene.remove(car);
        }

        lasthit[index] = new Date();
        lasthit[index].setSeconds(lasthit[index].getSeconds() + 3);

        timeCars[index] += 3.5;
    }
    return 0;
}

function animateCars(car, index) {
    if (Date.now() < lasthit[index] || end || countdown !=0)
        return;

    currangle[index] += (index + 1) * Math.PI / 1260;

    car.position.x = radius[index] * Math.cos(currangle[index])
    car.position.z = radius[index] * Math.sin(currangle[index])
    car.rotateY(-((index + 1) * Math.PI / 1260));

    checkCollision(car, index);
}

function  animate() {
    requestAnimationFrame(animate);

    if (countdown > 0)
        countdown_fn();

    if (keys['w'] && keys['s']) {
        Speed = 0.01;
        carSpeed += 0.01;
        fuel -= 0.1;
    }
    else if (keys['w']) {
        Speed = 0.06;
        carSpeed += Speed;
        fuel -= 0.06;
    }
    else if (keys['s']) {
        Speed = -0.04;
        carSpeed += Speed;
        fuel -= 0.04;
    }
    if (!keys['w'] && !keys['s']) {
        if (carSpeed > 0.03)
            carSpeed -= 0.03;
        else if (carSpeed < -0.03)
            carSpeed += 0.03;
        else
            carSpeed = 0;
    }

    if (carSpeed > maxSpeed) carSpeed = maxSpeed;
    if (-carSpeed > maxSpeed) carSpeed = -maxSpeed;

    if (keys['a'])
        rotateSpeed = Math.PI / 225;
    if (keys['d'])
        rotateSpeed = -Math.PI / 225;
    if ((!keys['a'] && !keys['d']) || (keys['a'] && keys['d']))
        rotateSpeed = 0;

    let x = camera_position1.x;
    let y = camera_position1.y;
    let z = camera_position1.z;
    let rot = camera.rotation.y;
    x += carSpeed * cameraDirection.x;
    y += carSpeed * cameraDirection.y;
    z += carSpeed * cameraDirection.z;
    if (carSpeed)
        rot += rotateSpeed;

    mcqueen.getWorldDirection(mcqueenDirection);
    mcqueenDirection.multiplyScalar(20);
    mcqueenDirection.add(new THREE.Vector3(x, y, z));

    let dist = Math.sqrt(Math.pow(mcqueenDirection.x, 2) + Math.pow(mcqueenDirection.z, 2));

    if (dist < 295 && dist > 215 && (fuel > 0 && health > 0 && !end && countdown==0)) {
        camera.position.x = keys['c'] ? x : mcqueenDirection.x;
        camera.position.y = keys['c'] ? y : 7;
        camera.position.z = keys['c'] ? z : mcqueenDirection.z;
        camera.rotation.y = rot;

        camera_position1.x = x;
        camera_position1.y = y;
        camera_position1.z = z;

        mcqueen.position.set(
            mcqueenDirection.x,
            0.1,
            mcqueenDirection.z
        );
        mcqueen.rotation.y = rot + Math.PI;

        if (carSpeed > 0)
            score += carSpeed;

        distance_travelled += Math.abs(carSpeed);
    }

    if (indexradiuschange2d * car2.position.x < 0) {
        indexradiuschange2d = -indexradiuschange2d;
        let rand = randomGenerator(0, 4);
        radius[0] = indexradiuschange1[rand][0];
        radius[1] = indexradiuschange1[rand][1];
        radius[2] = indexradiuschange2[rand][0];
        radius[3] = indexradiuschange2[rand][1];
    }

    if (mcqueen.position.z * checkpointz < 0) {
        checkpointz = -checkpointz;

        if (carSpeed > 0) {
            if (mcqueen.position.z > 0)
                checkpointsz[0] += 1;
            else
                checkpointsz[1] += 1;
        }
    }
    else if (mcqueen.position.x * checkpointx < 0) {
        checkpointx = -checkpointx;

        if (carSpeed > 0) {
            if (mcqueen.position.x > 0)
                checkpointsx[0] += 1;
            else
                checkpointsx[1] += 1;
        }
    }

    animateCars(car1, 0);
    animateCars(car2, 1);
    animateCars(car3, 2);
    animateCars(car4, 3);
    updateFuel();

    let insetHeight = window.innerHeight / 4;
    let insetWidth = window.innerWidth / 4;

    renderer.setViewport(0, 0, window.innerWidth, window.innerHeight);
    renderer.render(scene, camera);
    renderer.clearDepth();
    renderer.setScissorTest(true);
    renderer.setScissor(
        window.innerWidth - insetWidth - 16,
        window.innerHeight - insetHeight - 16,
        insetWidth,
        insetHeight
    );
    renderer.setViewport(
        window.innerWidth - insetWidth - 16,
        window.innerHeight - insetHeight - 16,
        insetWidth,
        insetHeight
    );
    renderer.render(scene, topview);
    renderer.setScissorTest(false);

    stats.update();
    updateStats();
}

function createStats() {
    var stats = new Stats();
    stats.setMode(0);

    stats.domElement.style.position = 'absolute';
    stats.domElement.style.left = '0';
    stats.domElement.style.top = '0';

    return stats;
}

function onDocumentKeyDown() {
    if (started) {
        camera.getWorldDirection(cameraDirection);
        let keyCode = event.key;
        if (keyCode == 'w' || keyCode == 'W')
            keys['w'] = true;

        if (keyCode == 's' || keyCode == 'S')
            keys['s'] = true;


        if (keyCode == 'a' || keyCode == 'A')
            keys['a'] = true;
        if (keyCode == 'd' || keyCode == 'D')
            keys['d'] = true;

        if (keyCode == 'c' || keyCode == "C")
            keys['c'] = !keys['c'];
    }
}

function onDocumentKeyUp() {
    if (started) {
        let keyCode = event.key;
        if (keyCode == 'w' || keyCode == 'W') {
            Speed = 0;
            keys['w'] = false;
        }
        if (keyCode == 's' || keyCode == 'S') {
            Speed = 0;
            keys['s'] = false;
        }

        if (keyCode == 'a' || keyCode == 'A') {
            keys['a'] = false;
            rotateSpeed = 0;
        }
        if (keyCode == 'd' || keyCode == 'D') {
            keys['d'] = false;
            rotateSpeed = 0;
        }
    } else if (event.key == " " ||
        event.code == "Space" ||
        event.keyCode == 32
    )
        start_game();
}

function start_game() {
    if (!started) {
        document.getElementById("start").style.display = "none";
        document.getElementById("game").style.display = "block";
        init();

        window.addEventListener('resize', () => onWindowResize(), false);
        // const controls = new OrbitControls(camera, renderer.domElement);

        started = true;
        start_time = new Date;
        animate();
    }
}

window.addEventListener('keydown', onDocumentKeyDown);
window.addEventListener('keyup', onDocumentKeyUp);
document.getElementById("start_button").addEventListener("click", start_game);

// https://coursehunters.online/t/threejs-journey-part-1/4409/1 (X)
// https://www.youtube.com/playlist?list=PLFky-gauhF46LALXSriZcXLJjwtZLjehn (X)
// https://www.youtube.com/watch?v=8SVxZS7h_NE (X)
// https://www.youtube.com/playlist?list=PLbu98QxRH81KkLTN00OXhD8Y-pRVgTCnM ()
// https://youtu.be/JhgBwJn1bQw?t=1539 (Partially)

// https://dustinpfister.github.io/2018/04/19/threejs-line/
// https://www.youtube.com/watch?v=9H3HPq-BTMo - Collision

// McQueen model link: https://sketchfab.com/3d-models/lightning-mcqueen-xbox-360-model-8c9f6ae266ad4e1c84e09933fd9a2075